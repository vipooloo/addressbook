#ifndef GROUPENTITY_H
#define GROUPENTITY_H

#include "AbstractEntity.h"
#include <string>
#include <vector>

class GroupEntity : public AbstractEntity
{
  public:
    GroupEntity()
      : GroupEntity(0, "")
    {}

    explicit GroupEntity(uint32_t rid)
      : GroupEntity(rid, "")
    {}

    explicit GroupEntity(const std::string& group_name)
      : GroupEntity(0, group_name)
    {}

    explicit GroupEntity(uint32_t rid, const std::string& group_name)
      : AbstractEntity(rid)
      , m_group_name{group_name}
    {
    }

    const std::string&
    GetGroupName() const
    {
        return m_group_name;
    }
    void SetGroupName(const std::string& group_name)
    {
        m_group_name = group_name;
    }
    void SetEmailRids(const std::string& email_rids)
    {
        m_email_rids = email_rids;
    }
    const std::string& GetEmailRids() const
    {
        return m_email_rids;
    }
    void SetEmailNames(const std::string& email_names)
    {
        m_email_names = email_names;
    }
    const std::string& GetEmailNames() const
    {
        return m_email_names;
    }

  private:
    std::string m_group_name;
    std::string m_email_rids;
    std::string m_email_names;
};

#endif  // GROUPENTITY_H