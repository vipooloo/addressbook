#include "EventLoop.h"

EventLoop::EventLoop(const EventHandlerFunc& handler_func)
  : m_thread(nullptr)
  , m_handler_func(handler_func)
  , m_is_running(false)
{
}

EventLoop::~EventLoop()
{
    Stop();
    if (m_thread)
    {
        m_thread.reset();
    }
}

void EventLoop::Start()
{
    if (!m_is_running)
    {
        m_is_running = true;
        m_thread.reset(new std::thread(&EventLoop::ThreadWorker, this));
    }
}

void EventLoop::Stop()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_is_running = false;
        m_cond_var.notify_all();
    }

    if (m_thread && m_thread->joinable())
    {
        m_thread->join();
    }
}

void EventLoop::PushEvent(const std::shared_ptr<IEvent>& event)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_event_queue.push(event);
    m_cond_var.notify_one();
}

void EventLoop::ThreadWorker()
{
    while (true)
    {
        std::shared_ptr<IEvent> evt_ptr = nullptr;

        {
            std::unique_lock<std::mutex> lock(m_mutex);

            // 循环等待，直到有数据或停止
            while (m_event_queue.empty() && m_is_running)
            {
                m_cond_var.wait(lock);
            }

            // 检查退出条件
            if (!m_is_running && m_event_queue.empty())
            {
                break;
            }

            // 取出事件
            if (!m_event_queue.empty())
            {
                evt_ptr = m_event_queue.front();
                m_event_queue.pop();
            }
        }

        if (m_handler_func)
        {
            m_handler_func(evt_ptr);
            evt_ptr = nullptr;
        }
    }
}