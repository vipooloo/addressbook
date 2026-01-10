#ifndef EMAILGROUPENTITY_H
#define EMAILGROUPENTITY_H

#include "AbstractEntity.h"
#include <cstdint>

class EmailGroupEntity : public AbstractEntity
{
  public:
    EmailGroupEntity()
      : EmailGroupEntity(0, 0)
    {}

    EmailGroupEntity(uint32_t mail_rid, uint32_t group_rid)
      : AbstractEntity(0)
      , m_mail_rid{mail_rid}
      , m_group_rid{group_rid}
    {}

    ~EmailGroupEntity() = default;

    uint32_t GetMailRid() const
    {
        return m_mail_rid;
    }
    void SetMailRid(uint32_t mail_rid)
    {
        m_mail_rid = mail_rid;
    }
    uint32_t GetGroupRid() const
    {
        return m_group_rid;
    }
    void SetGroupRid(uint32_t group_rid)
    {
        m_group_rid = group_rid;
    }

  private:
    uint32_t m_mail_rid;
    uint32_t m_group_rid;
};

#endif  // EMAILGROUPENTITY_H