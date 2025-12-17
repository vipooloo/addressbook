#ifndef EMAILENTITY_H
#define EMAILENTITY_H

#include "AbstractEntity.h"
#include <string>

class EmailEntity : public AbstractEntity
{
  public:
    EmailEntity()
      : EmailEntity(0, {}, {})
    {}

    explicit EmailEntity(uint32_t rid)
      : EmailEntity(rid, {}, {})
    {}

    EmailEntity(const std::string& email_address, const std::string& email_name)
      : EmailEntity(0, email_address, email_name)
    {}

    EmailEntity(const EmailEntity& other)
      : EmailEntity(other.GetRid(), other.m_email_address, other.m_email_name)
    {
    }

    EmailEntity(uint32_t rid, const std::string& email_address, const std::string& email_name)
      : AbstractEntity(rid)
      , m_email_address{email_address}
      , m_email_name{email_name}
    {
    }

    const std::string&
    GetEmailAddress() const
    {
        return m_email_address;
    }

    const std::string& GetEmailName() const
    {
        return m_email_name;
    }

    void SetEmailAddress(const std::string& email_address)
    {
        m_email_address = email_address;
    }

    void SetEmailName(const std::string& email_name)
    {
        m_email_name = email_name;
    }

  private:
    std::string m_email_address;
    std::string m_email_name;
};

#endif  // EMAILENTITY_H