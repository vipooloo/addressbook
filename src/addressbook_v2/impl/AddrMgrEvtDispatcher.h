#ifndef ADDRMGREVTDISPATCHER_H
#define ADDRMGREVTDISPATCHER_H

#include "IAddressMgrDataObserver.h"
#include <list>
#include <memory>
#include <mutex>

class AddrMgrEvtDispatcher
{
  public:
    AddrMgrEvtDispatcher();
    ~AddrMgrEvtDispatcher() = default;

    void Register(const std::shared_ptr<IAddressMgrDataObserver>& observer);

    void Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer);

    void Notify(ChangeType type);

  private:
    AddrMgrEvtDispatcher(const AddrMgrEvtDispatcher&) = delete;
    AddrMgrEvtDispatcher(AddrMgrEvtDispatcher&&) = delete;
    AddrMgrEvtDispatcher& operator=(const AddrMgrEvtDispatcher&) = delete;
    AddrMgrEvtDispatcher& operator=(AddrMgrEvtDispatcher&&) = delete;

  private:
    std::list<std::shared_ptr<IAddressMgrDataObserver>> m_observers;
    std::mutex m_mutex;
};

#endif  // ADDRMGREVTDISPATCHER_H