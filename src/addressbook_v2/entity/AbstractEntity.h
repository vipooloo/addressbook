#ifndef ABSTRACTENTITY_H
#define ABSTRACTENTITY_H

#include <stdint.h>

class AbstractEntity
{
  public:
    explicit AbstractEntity(uint32_t rid)
      : m_rid{rid}
    {
    }
    virtual ~AbstractEntity() = default;
    uint32_t GetRid() const
    {
        return m_rid;
    }
    void SetRid(uint32_t rid)
    {
        m_rid = rid;
    }

  private:
    uint32_t m_rid;
};

#endif  // ABSTRACTENTITY_H