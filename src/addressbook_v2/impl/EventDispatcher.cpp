#include "EventDispatcher.h"
#include <algorithm>

namespace addrbook {
EventDispatcher::EventDispatcher()
  : m_observers{}
  , m_mutex{}
{
}

bool EventDispatcher::Register(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    bool result = false;
    std::lock_guard<std::mutex> lock(m_mutex);
    std::list<std::shared_ptr<IAddressMgrDataObserver>>::const_iterator it = std::find(m_observers.cbegin(), m_observers.cend(), observer);
    if (it == m_observers.end())
    {
        m_observers.emplace_back(observer);
        result = true;
    }
    return result;
}

bool EventDispatcher::Unregister(const std::shared_ptr<IAddressMgrDataObserver>& observer)
{
    bool result = false;
    std::lock_guard<std::mutex> lock(m_mutex);
    std::list<std::shared_ptr<IAddressMgrDataObserver>>::iterator it = std::remove_if(m_observers.begin(), m_observers.end(), [&observer](const std::shared_ptr<IAddressMgrDataObserver>& sptr) {
        return sptr == observer;
    });
    if (it != m_observers.end())
    {
        m_observers.erase(it);
        result = true;
    }
    return result;
}

void EventDispatcher::Notify(ChangeType type)
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
}  // namespace addrbook