#ifndef IEVENT_H
#define IEVENT_H

#include <cstdint>

class IEvent
{
  public:
    IEvent() = default;
    virtual ~IEvent() = default;
};

enum class EventType : uint8_t
{
    EMail_Add = 0x00 | 0x00,
    EMail_Delete,
    EMail_Update,
    EMail_Search_By_Name,

    Group_Add = 0x01 | 0x00,
};

class AbstractEvent : public IEvent
{
  public:
    explicit AbstractEvent(EventType type)
      : m_type{type}
    {}
    ~AbstractEvent() = default;
    EventType GetType() const
    {
        return m_type;
    }

  private:
    EventType m_type;
};
#endif  // IEVENT_H