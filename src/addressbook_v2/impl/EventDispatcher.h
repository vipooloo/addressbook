#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "IAddressMgrDataObserver.h"
#include <list>
#include <memory>
#include <mutex>

namespace addrbook {
class EventDispatcher
{
  public:
    EventDispatcher();
    ~EventDispatcher() = default;

    void Register(const std::shared_ptr<IAddressMgrDataObserver>& observer);

    void Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer);

    void Notify(ChangeType type);

  private:
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher(EventDispatcher&&) noexcept = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
    EventDispatcher& operator=(EventDispatcher&&) noexcept = delete;

  private:
    std::list<std::shared_ptr<IAddressMgrDataObserver>> m_observers;
    std::mutex m_mutex;
};
}  // namespace addrbook

#endif  // EVENTDISPATCHER_H