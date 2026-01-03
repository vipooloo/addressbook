#include "SyncWaiter.h"

SyncWaiter::SyncWaiter()
  : m_mutex{}
  , m_cond_var{}
  , m_is_signaled{false}
{
}

void SyncWaiter::Wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond_var.wait(lock, [this]() {
        return m_is_signaled;
    });
}

void SyncWaiter::Notify()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_is_signaled = true;
    }
    m_cond_var.notify_one();
}
