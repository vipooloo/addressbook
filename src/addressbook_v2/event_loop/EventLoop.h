#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "IEvent.h"
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

using EventHandlerFunc = std::function<void(const std::shared_ptr<IEvent>&)>;

class EventLoop
{
  public:
    explicit EventLoop(const EventHandlerFunc& handler_func);
    ~EventLoop();

    // 启动事件循环线程
    void Start();

    // 停止事件循环
    void Stop();

    // 添加事件到队列
    void PushEvent(const std::shared_ptr<IEvent>& event);

  private:
    void ThreadWorker();

  private:
    std::queue<std::shared_ptr<IEvent>> m_event_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond_var;
    std::unique_ptr<std::thread> m_thread;
    EventHandlerFunc m_handler_func;
    bool m_is_running;
};

#endif  // EVENTLOOP_H