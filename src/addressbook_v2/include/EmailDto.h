#ifndef EMAILDTO_H
#define EMAILDTO_H

#include <cstdint>
#include <string>
#include <vector>

class EmailDto
{
  public:
    EmailDto()
      : EmailDto(0, "", "", {})
    {
    }
    EmailDto(const std::string& address, const std::string& name)
      : EmailDto(0, address, name, {}, {})
    {
    }
    EmailDto(const std::string& address, const std::string& name, const std::vector<uint32_t>& group_rids)
      : EmailDto(0, address, name, group_rids)
    {
    }
    EmailDto(uint32_t rid, const std::string& address, const std::string& name)
      : EmailDto(rid, address, name, {})
    {
    }
    EmailDto(uint32_t rid, const std::string& address, const std::string& name, const std::vector<uint32_t>& group_rids)
      : EmailDto(rid, address, name, group_rids, {})
    {}
    EmailDto(const std::string& address, const std::string& name, const std::vector<std::string>& group_names)
      : EmailDto(0, address, name, {}, group_names)
    {}
    EmailDto(uint32_t rid, const std::string& address, const std::string& name, const std::vector<uint32_t>& group_rids, const std::vector<std::string>& group_names)
      : m_rid{rid}
      , m_address{address}
      , m_name{name}
      , m_group_rids{group_rids}
      , m_group_names{group_names}
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

    const std::vector<uint32_t>& GetGroupRids() const
    {
        return m_group_rids;
    }
    void SetGroupRids(const std::vector<uint32_t>& group_rids)
    {
        m_group_rids = group_rids;
    }
    const std::vector<std::string>& GetGroupNames() const
    {
        return m_group_names;
    }
    void SetGroupNames(const std::vector<std::string>& group_names)
    {
        m_group_names = group_names;
    }

  private:
    uint32_t m_rid;
    std::string m_address;
    std::string m_name;
    std::vector<uint32_t> m_group_rids;
    std::vector<std::string> m_group_names;
};

#endif  // EMAILDTO_H