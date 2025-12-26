#ifndef GROUPENTITY_H
#define GROUPENTITY_H

#include "AbstractEntity.h"
#include <string>
#include <vector>

class GroupEntity : public AbstractEntity
{
  public:
    GroupEntity()
      : GroupEntity(0, {})
    {}

    explicit GroupEntity(uint32_t rid)
      : GroupEntity(rid, {})
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

  private:
    std::string m_group_name;
};

#endif  // GROUPENTITY_H