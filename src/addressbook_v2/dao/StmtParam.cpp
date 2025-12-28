#include "StmtParam.h"
#include <cstring>

StmtParam::StmtParam(uint32_t val)
  : m_type{StmtParamType::UINT32}
  , m_data{0, 0, 0, 0}
{
  std::memcpy(m_data.data(), &val, sizeof(val));
}

StmtParam::StmtParam(int32_t val)
  : m_type{StmtParamType::INT32}
  , m_data{0, 0, 0, 0}
{
  std::memcpy(m_data.data(), &val, sizeof(val));
}

StmtParam::StmtParam(const std::string& val)
  : m_type{StmtParamType::STRING}
  , m_data{val.cbegin(), val.cend()}
{
}

int32_t StmtParam::GetInt32Value() const
{
    int32_t ret = 0;
    std::memcpy(&ret, m_data.data(), sizeof(ret));
    return ret;
}

uint32_t StmtParam::GetUInt32Value() const
{
    uint32_t ret = 0;
    std::memcpy(&ret, m_data.data(), sizeof(ret));
    return ret;
}

std::string StmtParam::GetStringValue() const
{
    std::string ret(m_data.cbegin(), m_data.cend());
    return ret;
}
