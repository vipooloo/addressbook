#include "AddrMgrEvtDispatcher.h"
#include <algorithm>

AddrMgrEvtDispatcher::AddrMgrEvtDispatcher()
  : m_observers{}
  , m_mutex{}
{
}

void AddrMgrEvtDispatcher::Register(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::list<std::shared_ptr<IAddressMgrDataObserver>>::const_iterator it = std::find(m_observers.cbegin(), m_observers.cend(), observer);
    if (it == m_observers.end())
    {
        m_observers.emplace_back(observer);
    }
}

void AddrMgrEvtDispatcher::Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::remove_if(m_observers.begin(), m_observers.end(), [&observer](const std::shared_ptr<IAddressMgrDataObserver>& sptr) {
        return sptr == observer;
    });
}

void AddrMgrEvtDispatcher::Notify(ChangeType type)
{
    std::list<std::shared_ptr<IAddressMgrDataObserver>> observers_copy;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        observers_copy = m_observers;
    }
    for (const std::shared_ptr<IAddressMgrDataObserver>& obs_sptr : observers_copy)
    {
        if (obs_sptr)
        {
            obs_sptr->OnChanged(type);
        }
    }
}