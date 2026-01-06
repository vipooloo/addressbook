#include "AddressMgrEvtDispatcher.h"
#include <algorithm>

AddressMgrEvtDispatcher::AddressMgrEvtDispatcher()
  : m_observers{}
  , m_mutex{}
{
}

void AddressMgrEvtDispatcher::Register(const std::shared_ptr<IAddressDataObserver>& observer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::list<std::shared_ptr<IAddressDataObserver>>::const_iterator it = std::find(m_observers.cbegin(), m_observers.cend(), observer);
    if (it == m_observers.end())
    {
        m_observers.emplace_back(observer);
    }
}

void AddressMgrEvtDispatcher::Unregister(const std::shared_ptr<IAddressDataObserver>& observer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::remove_if(m_observers.begin(), m_observers.end(), [&observer](const std::shared_ptr<IAddressDataObserver>& sptr) {
        return sptr == observer;
    });
}

void AddressMgrEvtDispatcher::Notify(ChangeType type)
{
    std::list<std::shared_ptr<IAddressDataObserver>> observers_copy;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        observers_copy = m_observers;
    }
    for (const std::shared_ptr<IAddressDataObserver>& obs_sptr : observers_copy)
    {
        if (obs_sptr)
        {
            obs_sptr->OnChanged(type);
        }
    }
}