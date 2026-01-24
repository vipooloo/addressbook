#ifndef GROUPDTO_H
#define GROUPDTO_H

#include <cstdint>
#include <string>
#include <vector>

class GroupDto
{
  public:
    GroupDto()
      : GroupDto(0, {}, {})
    {}

    explicit GroupDto(const std::string& group_name)
      : GroupDto(0, group_name, {})
    {}
    GroupDto(const std::string& group_name, const std::vector<uint32_t>& mail_rids)
      : GroupDto(0, group_name, mail_rids)
    {}
    GroupDto(uint32_t rid, const std::string& group_name, const std::vector<uint32_t>& mail_rids)
      : GroupDto(rid, group_name, mail_rids, {})
    {
    }
    GroupDto(uint32_t rid, const std::string& group_name, const std::vector<uint32_t>& mail_rids, const std::vector<std::string>& mail_names)
      : m_rid{rid}
      , m_group_name{group_name}
      , m_email_rids{mail_rids}
      , m_email_names{mail_names}
    {
    }
    void SetRid(uint32_t rid)
    {
        m_rid = rid;
    }
    uint32_t GetRid() const
    {
        return m_rid;
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
    std::vector<uint32_t> GetMailRids() const
    {
        return m_email_rids;
    }
    void SetMailRids(const std::vector<uint32_t>& ids)
    {
        m_email_rids = ids;
    }
    std::vector<std::string> GetMailNames() const
    {
        return m_email_names;
    }
    void SetMailNames(const std::vector<std::string>& names)
    {
        m_email_names = names;
    }

  private:
    uint32_t m_rid;
    std::string m_group_name;
    std::vector<uint32_t> m_email_rids;
    std::vector<std::string> m_email_names;
};

#endif  // GROUPDTO_H~