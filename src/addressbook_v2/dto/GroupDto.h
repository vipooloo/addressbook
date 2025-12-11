#ifndef GROUPDTO_H
#define GROUPDTO_H

#include <stdint.h>
#include <string>

class GroupDto
{
  public:
    GroupDto()
      : GroupDto(0, {})
    {}

    explicit GroupDto(uint32_t rid)
      : GroupDto(rid, {})
    {}

    explicit GroupDto(uint32_t rid, const std::string& group_name)
      : m_group_name{m_group_name}
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

  private:
    std::string m_group_name;
};

#endif  // GROUPDTO_H