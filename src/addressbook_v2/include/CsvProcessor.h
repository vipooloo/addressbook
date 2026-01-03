#ifndef CSV_PROCESSOR_H
#define CSV_PROCESSOR_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

enum CsvStatus
{
    CSV_SUCCESS = 0,
    CSV_ERROR_FILE_OPEN = 1,
    CSV_ERROR_HEADER_MISMATCH = 2,
    CSV_ERROR_PARSE = 3,
    CSV_ERROR_WRITE = 4,
    CSV_EOF = 5,
    CSV_ERROR_INIT_FAILED = 6
};

// --- 基类：包含公共逻辑 ---
class CsvBase
{
  public:
    virtual ~CsvBase() = default;

    // 静态工具函数
    static size_t GetFileSize(const std::string& file_name);

  protected:
    CsvBase(const std::string& file_name, const std::vector<std::string>& header_list)
      : m_file_name(file_name)
      , m_fixed_headers(header_list)
      , m_init_status(CSV_SUCCESS)
    {}

    std::string EscapeCell(const std::string& cell_data);
    void ParseLine(const std::string& line_str, std::vector<std::string>& row_out);

    const std::string& GetFilename() const
    {
        return m_file_name;
    }
    const std::vector<std::string>& GetFixedHeaders() const
    {
        return m_fixed_headers;
    }
    void SetInitStatus(CsvStatus status)
    {
        m_init_status = status;
    }
    CsvStatus GetInitStatusVal() const
    {
        return m_init_status;
    }

  private:
    // 禁止拷贝和赋值
    CsvBase(const CsvBase&) = delete;
    CsvBase(CsvBase&&) = delete;
    CsvBase& operator=(const CsvBase&) = delete;
    CsvBase& operator=(CsvBase&&) = delete;

  private:
    std::string m_file_name;
    std::vector<std::string> m_fixed_headers;
    CsvStatus m_init_status;
};

class CsvReader : public CsvBase
{
  public:
    CsvReader(const std::string& file_name, const std::vector<std::string>& header_list);
    ~CsvReader() override;

    CsvStatus ReadBatch(uint32_t max_rows, std::vector<std::vector<std::string>>& batch_out);

  private:
    std::ifstream m_ifs;
};

class CsvWriter : public CsvBase
{
  public:
    // is_append: true为追加，false为覆盖
    CsvWriter(const std::string& file_name, const std::vector<std::string>& header_list);
    ~CsvWriter() override;

    CsvStatus WriteBatch(const std::vector<std::vector<std::string>>& batch_data);

  private:
    std::ofstream m_ofs;
};

#endif  // CSV_PROCESSOR_H