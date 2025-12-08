#ifndef EMAILDTO_H
#define EMAILDTO_H

#include <stdint.h>
#include <string>

class EmailDto
{
  public:
    EmailDto(const std::string& address, const std::string& name)
      : m_rid{0}
      , m_address{address}
      , m_name{name}
    {
    }
    ~EmailDto() = default;

    uint32_t GetRid() const
    {
        return m_rid;
    }
    void SetRid(uint32_t rid)
    {
        m_rid = rid;
    }

    const std::string& GetAddress() const
    {
        return m_address;
    }
    void SetAddress(const std::string& address)
    {
        m_address = address;
    }

    const std::string& GetName() const
    {
        return m_name;
    }
    void SetName(const std::string& name)
    {
        m_name = name;
    }

  private:
    uint32_t m_rid;
    std::string m_address;
    std::string m_name;
};

#endif  // EMAILDTO_H