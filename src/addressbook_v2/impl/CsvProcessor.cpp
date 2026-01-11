#include "CsvProcessor.h"
#include <iostream>

// ==========================================
// CsvBase 实现
// ==========================================
namespace addrbook {

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

    row_out.clear();

    for (size_t i = 0; i < len; ++i)
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
            else if (c != '\r' && c != '\n')
            {
                current_cell.push_back(c);
            }
            else
            {}
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

CsvStatus CsvReader::ReadNextRow(std::vector<std::string>& row_out)
{
    CsvStatus current_status = CSV_SUCCESS;

    // 1. 基础状态检查
    if (GetInitStatusVal() != CSV_SUCCESS)
    {
        current_status = CSV_ERROR_INIT_FAILED;
    }
    else if (!m_ifs.is_open())
    {
        current_status = CSV_ERROR_FILE_OPEN;
    }
    else
    {
        // 2. 准备读取变量
        std::string line_str = "";
        const std::vector<std::string>& fixed_headers = GetFixedHeaders();
        size_t header_count = fixed_headers.size();
        bool found_valid_row = false;

        // 默认状态设为读取错误，只有成功解析或正常EOF时才修改
        current_status = CSV_ERROR_READ;

        // 3. 循环读取
        while (std::getline(m_ifs, line_str))
        {
            if (line_str.empty())
            {
                continue;
            }
            // 处理 Windows 换行符
            if (line_str.back() == '\r')
            {
                line_str.pop_back();
            }
            if (line_str.empty())
            {
                continue;
            }
            ParseLine(line_str, row_out);
            // 校验列数
            if (row_out.size() == header_count)
            {
                current_status = CSV_SUCCESS;
                found_valid_row = true;
                // 找到有效行，跳出循环
                break;
            }
            // 如果列数不匹配，视为脏数据，继续下一次循环
        }

        // 4. 循环结束后的状态修正
        // 如果没找到有效行，且并未发生错误（仅仅是读完了），则修正为 EOF
        if (!found_valid_row)
        {
            if (m_ifs.eof())
            {
                current_status = CSV_EOF;
            }
        }
    }

    return current_status;
}
// ==========================================
// CsvWriter 实现
// ==========================================

CsvWriter::CsvWriter(const std::string& file_name, const std::vector<std::string>& header_list)
  : CsvBase(file_name, header_list)
  , m_ofs{}
{
    std::ios_base::openmode mode = std::ios::out;
    bool has_content = GetFileSize(file_name) > 0;
    if (has_content)
    {
        mode |= std::ios::app;
    }
    else
    {
        mode |= std::ios::trunc;
    }

    m_ofs.open(GetFilename().c_str(), mode);

    if (m_ofs.is_open())
    {
        // 3. 只有在新文件（或空文件）模式下，才写入表头
        if (!has_content)
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
}  // namespace addrbook