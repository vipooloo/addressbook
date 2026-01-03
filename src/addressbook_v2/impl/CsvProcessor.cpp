#include "CsvProcessor.h"
#include <iostream>

// ==========================================
// CsvBase 实现
// ==========================================

size_t CsvBase::GetFileSize(const std::string& file_name)
{
    size_t ret = 0;
    std::ifstream temp_ifs(file_name.c_str(), std::ios::binary | std::ios::ate);

    if (temp_ifs.is_open())
    {
        ret = static_cast<size_t>(temp_ifs.tellg());
    }
    return ret;
}

std::string CsvBase::EscapeCell(const std::string& cell_data)
{
    std::string result_str = "";
    const char* special_chars = ",\"\n\r";
    size_t len = cell_data.length();
    size_t found_pos = cell_data.find_first_of(special_chars);

    if (found_pos == std::string::npos)
    {
        // 分支1：不需要转义，直接赋值
        result_str = cell_data;
    }
    else
    {
        // 分支2：需要转义，执行拼接逻辑
        result_str.reserve(len + 8);
        result_str.push_back('"');

        for (size_t i = 0; i < len; ++i)
        {
            char c = cell_data[i];
            if (c == '"')
            {
                result_str.push_back('"');
                result_str.push_back('"');
            }
            else
            {
                result_str.push_back(c);
            }
        }
        result_str.push_back('"');
    }
    return result_str;
}

void CsvBase::ParseLine(const std::string& line_str, std::vector<std::string>& row_out)
{
    std::string current_cell = "";
    bool in_quotes = false;
    size_t len = line_str.length();
    size_t i = 0;

    row_out.clear();

    for (i = 0; i < len; ++i)
    {
        char c = line_str[i];
        if (in_quotes)
        {
            // 处理引号内的情况
            if (c == '"')
            {
                // 检查双引号转义
                if (i + 1 < len && line_str[i + 1] == '"')
                {
                    current_cell.push_back('"');
                    i = i + 1;
                }
                else
                {
                    in_quotes = false;
                }
            }
            else
            {
                current_cell.push_back(c);
            }
        }
        else
        {
            // 处理引号外的情况
            if (c == '"')
            {
                in_quotes = true;
            }
            else if (c == ',')
            {
                row_out.push_back(current_cell);
                current_cell.clear();
            }
            else
            {
                if (c != '\r' && c != '\n')
                {
                    current_cell.push_back(c);
                }
            }
        }
    }
    // 添加最后一个单元格
    row_out.push_back(current_cell);
}

// ==========================================
// CsvReader 实现
// ==========================================

CsvReader::CsvReader(const std::string& file_name, const std::vector<std::string>& header_list)
  : CsvBase(file_name, header_list)
  , m_ifs{}
{
    do
    {
        // 1. 尝试打开文件
        m_ifs.open(GetFilename().c_str(), std::ios::in);
        if (!m_ifs.is_open())
        {
            SetInitStatus(CSV_ERROR_FILE_OPEN);
            break;
        }

        std::string line_str = "";
        // 2. 读取第一行
        if (!std::getline(m_ifs, line_str))
        {
            SetInitStatus(CSV_ERROR_PARSE);
            break;
        }

        // 3. 处理 Windows 换行符
        if (!line_str.empty() && line_str[line_str.size() - 1] == '\r')
        {
            line_str.erase(line_str.size() - 1);
        }

        std::vector<std::string> header_row;
        ParseLine(line_str, header_row);
        const std::vector<std::string>& fixed_headers = GetFixedHeaders();

        // 4. 校验列数
        if (header_row.size() != fixed_headers.size())
        {
            SetInitStatus(CSV_ERROR_HEADER_MISMATCH);
            break;
        }

        // 5. 校验表头名称
        for (size_t i = 0; i < fixed_headers.size(); ++i)
        {
            if (header_row[i] != fixed_headers[i])
            {
                SetInitStatus(CSV_ERROR_HEADER_MISMATCH);
                break;
            }
        }
    } while (false);
}

CsvReader::~CsvReader()
{
    if (m_ifs.is_open())
    {
        m_ifs.close();
    }
}

CsvStatus CsvReader::ReadBatch(uint32_t max_rows, std::vector<std::vector<std::string>>& batch_out)
{
    CsvStatus current_status = CSV_SUCCESS;

    do
    {
        if (GetInitStatusVal() != CSV_SUCCESS)
        {
            current_status = CSV_ERROR_INIT_FAILED;
            break;
        }

        if (!m_ifs.is_open())
        {
            current_status = CSV_ERROR_FILE_OPEN;
            break;
        }

        batch_out.clear();
        std::string line_str = "";
        uint32_t count = 0;
        const std::vector<std::string>& fixed_headers = GetFixedHeaders();

        while (count < max_rows && std::getline(m_ifs, line_str))
        {
            if (line_str.empty())
            {
                continue;
            }
            if (line_str[line_str.size() - 1] == '\r')
            {
                line_str.erase(line_str.size() - 1);
            }
            if (line_str.empty())
            {
                continue;
            }

            std::vector<std::string> row;
            ParseLine(line_str, row);

            if (row.size() == fixed_headers.size())
            {
                batch_out.push_back(row);
                count = count + 1;
            }
        }

        // 检查 EOF 状态
        if (batch_out.empty() && m_ifs.eof())
        {
            current_status = CSV_EOF;
            break;
        }

    } while (false);

    return current_status;
}

// ==========================================
// CsvWriter 实现
// ==========================================

CsvWriter::CsvWriter(const std::string& file_name, const std::vector<std::string>& header_list, bool is_append)
  : CsvBase(file_name, header_list)
  , m_ofs{}
{
    // 如果是追加模式使用 app，否则使用 trunc (覆盖)
    std::ios_base::openmode mode = std::ios::out;
    if (is_append)
    {
        mode |= std::ios::app;
    }
    else
    {
        mode |= std::ios::trunc;
    }

    m_ofs.open(GetFilename().c_str(), mode);

    // 【关键修正】这里之前的逻辑反了
    if (m_ofs.is_open())
    {
        // 如果不是追加模式，则需要写入表头
        if (!is_append)
        {
            const std::vector<std::string>& fixed_headers = GetFixedHeaders();
            for (size_t i = 0; i < fixed_headers.size(); ++i)
            {
                m_ofs << EscapeCell(fixed_headers[i]);
                if (i < fixed_headers.size() - 1)
                {
                    m_ofs << ",";
                }
            }
            m_ofs << "\n";
        }
    }
    else
    {
        SetInitStatus(CSV_ERROR_FILE_OPEN);
    }
}

CsvWriter::~CsvWriter()
{
    if (m_ofs.is_open())
    {
        m_ofs.close();
    }
}

CsvStatus CsvWriter::WriteBatch(const std::vector<std::vector<std::string>>& batch_data)
{
    CsvStatus current_status = CSV_SUCCESS;
    do
    {
        if (GetInitStatusVal() != CSV_SUCCESS)
        {
            current_status = CSV_ERROR_INIT_FAILED;
            break;
        }

        if (!m_ofs.is_open())
        {
            current_status = CSV_ERROR_FILE_OPEN;
            break;
        }

        const std::vector<std::string>& fixed_headers = GetFixedHeaders();
        for (size_t i = 0; i < batch_data.size(); ++i)
        {
            const std::vector<std::string>& row = batch_data[i];

            if (row.size() != fixed_headers.size())
            {
                // 忽略列数不匹配的行
                continue;
            }

            for (size_t j = 0; j < row.size(); ++j)
            {
                m_ofs << EscapeCell(row[j]);

                if (j < row.size() - 1)
                {
                    m_ofs << ",";
                }
            }
            m_ofs << "\n";
        }

        if (m_ofs.fail())
        {
            current_status = CSV_ERROR_WRITE;
            break;
        }

    } while (false);

    return current_status;
}