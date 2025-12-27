#ifndef STMTPARAM_H
#define STMTPARAM_H

#include <cstdint>
#include <string>
#include <vector>

enum class StmtParamType
{
    INT32,
    UINT32,
    STRING,
    BLOB,
};

class StmtParam
{
  public:
    explicit StmtParam(uint32_t val);
    explicit StmtParam(int32_t val);
    explicit StmtParam(const std::string& val);
    ~StmtParam() = default;

    StmtParamType GetType() const
    {
        return m_type;
    }
    int32_t GetInt32Value() const;
    uint32_t GetUInt32Value() const;
    std::string GetStringValue() const;

  private:
    StmtParamType m_type;
    std::vector<uint8_t> m_data;
};

#endif  // STMTPARAM_H