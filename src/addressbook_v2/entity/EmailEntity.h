#ifndef EMAILENTITY_H
#define EMAILENTITY_H

#include "AbstractEntity.h"
#include <string>

class EmailEntity : public AbstractEntity
{
  public:
    EmailEntity()
      : EmailEntity(0, "", "")
    {}

    explicit EmailEntity(uint32_t rid)
      : EmailEntity(rid, "", "")
    {}

    EmailEntity(const std::string& email_address, const std::string& email_name)
      : EmailEntity(0, email_address, email_name)
    {}

    EmailEntity(const EmailEntity& other)
      : EmailEntity(other.GetRid(), other.m_email_address, other.m_email_name)
    {
    }
    EmailEntity(uint32_t rid, const std::string& email_address, const std::string& email_name)
      : EmailEntity(rid, email_address, email_name, {}, {})
    {}
    EmailEntity(uint32_t rid, const std::string& email_address, const std::string& email_name, const std::string& group_rids, const std::string& group_names)
      : AbstractEntity(rid)
      , m_email_address{email_address}
      , m_email_name{email_name}
      , m_group_rids{group_rids}
      , m_group_names{group_names}
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
    void SetGroupRids(const std::string& group_rids)
    {
        m_group_rids = group_rids;
    }
    void SetGroupNames(const std::string& group_names)
    {
        m_group_names = group_names;
    }
    const std::string& GetGroupRids() const
    {
        return m_group_rids;
    }
    const std::string& GetGroupNames() const
    {
        return m_group_names;
    }

  private:
    std::string m_email_address;
    std::string m_email_name;
    std::string m_group_rids;
    std::string m_group_names;
};

#endif  // EMAILENTITY_H