#ifndef ADDRESSMGREVTDISPATCHER_H
#define ADDRESSMGREVTDISPATCHER_H

#include "IAddressDataObserver.h"
#include <list>
#include <memory>
#include <mutex>

class AddressMgrEvtDispatcher
{
  public:
    AddressMgrEvtDispatcher();
    ~AddressMgrEvtDispatcher() = default;

    void Register(const std::shared_ptr<IAddressDataObserver>& observer);

    void Unregister(const std::shared_ptr<IAddressDataObserver>& observer);

    void Notify(ChangeType type);

  private:
    AddressMgrEvtDispatcher(const AddressMgrEvtDispatcher&) = delete;
    AddressMgrEvtDispatcher(AddressMgrEvtDispatcher&&) = delete;
    AddressMgrEvtDispatcher& operator=(const AddressMgrEvtDispatcher&) = delete;
    AddressMgrEvtDispatcher& operator=(AddressMgrEvtDispatcher&&) = delete;

  private:
    std::list<std::shared_ptr<IAddressDataObserver>> m_observers;
    std::mutex m_mutex;
};

#endif  // ADDRESSMGREVTDISPATCHER_H