#ifndef GROUPDTO_H
#define GROUPDTO_H

#include <stdint.h>
#include <string>

class GroupDto
{
  public:
    GroupDto()
      : GroupDto(0, {}, {})
    {}

    explicit GroupDto(uint32_t rid)
      : GroupDto(rid, {}, {})
    {}
    explicit GroupDto(const std::string& group_name)
      : GroupDto(0, group_name, {})
    {}

    GroupDto(uint32_t rid, const std::string& group_name, const std::vector<uint32_t>& mail_rids)
      : m_rid{rid}
      , m_group_name{group_name}
      , m_mail_rids{mail_rids}
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
        return m_mail_rids;
    }
    void SetMailRids(const std::vector<uint32_t>& rids)
    {
        m_mail_rids = rids;
    }

  private:
    uint32_t m_rid;
    std::string m_group_name;
    std::vector<uint32_t> m_mail_rids;
};

#endif  // GROUPDTO_H