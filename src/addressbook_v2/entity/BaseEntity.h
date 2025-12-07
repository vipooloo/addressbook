#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <stdint.h>

class BaseEntity
{
  public:
    BaseEntity(uint32_t rid)
      : m_rid{rid}
    {
    }
    virtual ~BaseEntity() = default;
    uint32_t GetRid() const
    {
        return m_rid;
    }

  private:
    uint32_t m_rid;
};

#endif  // BASEENTITY_H