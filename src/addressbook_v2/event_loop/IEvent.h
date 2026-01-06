#ifndef IEVENT_H
#define IEVENT_H

#include "AddressManagerDefs.h"
#include <cstdint>
#include <string>

class IEvent
{
  public:
    IEvent() = default;
    virtual ~IEvent() = default;
};

enum class EventType : uint8_t
{
    EMail_Import = 0x00 | 0x00,
    EMail_Export,

    Group_Import = 0x01 | 0x00,
};

class AbstractEvent : public IEvent
{
  public:
    explicit AbstractEvent(EventType type)
      : IEvent()
      , m_type{type}
    {}
    ~AbstractEvent() = default;
    EventType GetType() const
    {
        return m_type;
    }

  private:
    EventType m_type;
};

class ImportExportEvent : public AbstractEvent
{
  public:
    ImportExportEvent(EventType type, const std::string& file_path, const ImportExportCallback& cb)
      : AbstractEvent{type}
      , m_file_path{file_path}
      , m_cb{cb}
    {}
    ~ImportExportEvent() = default;

    const std::string& GetFilePath() const
    {
        return m_file_path;
    }
    ImportExportCallback GetCallback() const
    {
        return m_cb;
    }

  private:
    std::string m_file_path;
    ImportExportCallback m_cb;
};
#endif  // IEVENT_H