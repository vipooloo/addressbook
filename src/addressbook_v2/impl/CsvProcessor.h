#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace addrbook {

enum CsvStatus : uint8_t
{
    CSV_SUCCESS = 0,
    CSV_ERROR_FILE_OPEN,
    CSV_ERROR_HEADER_MISMATCH,
    CSV_ERROR_READ,
    CSV_ERROR_PARSE,
    CSV_ERROR_WRITE,
    CSV_EOF,
    CSV_ERROR_INIT_FAILED
};

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

    static std::string EscapeCell(const std::string& cell_data);
    static void ParseLine(const std::string& line_str, std::vector<std::string>& row_out);

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
    CsvBase(CsvBase&&) noexcept = delete;
    CsvBase& operator=(const CsvBase&) = delete;
    CsvBase& operator=(CsvBase&&) noexcept = delete;

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

    CsvStatus ReadNextRow(std::vector<std::string>& row_out);

  private:
    std::ifstream m_ifs;
};

class CsvWriter : public CsvBase
{
  public:
    CsvWriter(const std::string& file_name, const std::vector<std::string>& header_list);
    ~CsvWriter() override;

    CsvStatus WriteBatch(const std::vector<std::vector<std::string>>& batch_data);

  private:
    std::ofstream m_ofs;
};

}  // namespace addrbook

#endif  // CSVPROCESSOR_H