#ifndef ADDREVENTDISPATCHER_H
#define ADDREVENTDISPATCHER_H

#include "IAddrCenterDataObserver.h"
#include <list>
#include <memory>
#include <mutex>

class AddrEventDispatcher
{
  public:
    AddrEventDispatcher();
    ~AddrEventDispatcher() = default;

    void Register(const std::shared_ptr<IAddrCenterDataObserver>& observer);

    void Unregister(const std::shared_ptr<IAddrCenterDataObserver>& observer);

    void Notify(ChangeType type);

  private:
    AddrEventDispatcher(const AddrEventDispatcher&) = delete;
    AddrEventDispatcher(AddrEventDispatcher&&) = delete;
    AddrEventDispatcher& operator=(const AddrEventDispatcher&) = delete;
    AddrEventDispatcher& operator=(AddrEventDispatcher&&) = delete;

  private:
    std::list<std::shared_ptr<IAddrCenterDataObserver>> m_observers;
    std::mutex m_mutex;
};

#endif  // ADDREVENTDISPATCHER_H