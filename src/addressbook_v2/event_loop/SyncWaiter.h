#ifndef SYNCWAITER_H
#define SYNCWAITER_H

#include <condition_variable>
#include <cstdint>
#include <mutex>

class SyncWaiter
{
  public:
    SyncWaiter();
    ~SyncWaiter() = default;

    void Wait();

    void Notify();

  private:
    std::mutex m_mutex;
    std::condition_variable m_cond_var;
    bool m_is_signaled;
};

#endif  // SYNCWAITER_H