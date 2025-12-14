#ifndef MAILGROUPRELATION_H
#define MAILGROUPRELATION_H

#include "AbstractEntity.h"
#include <stdint.h>

class MailGroupRelation : public AbstractEntity
{
  public:
    MailGroupRelation()
      : AbstractEntity(0)
      , m_mail_rid{0}
      , m_group_rid{0} {};
    ~MailGroupRelation() = default;

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

#endif  // MAILGROUPRELATION_H