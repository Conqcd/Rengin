#include "repch.hpp"
#include "ThreadPool.hpp"

namespace Rengin
{
    
ThreadPool::ThreadPool(int size)
    : m_Stop(false)
{
    for (int i = 0; i < size; i++)
    {
        m_Workers.emplace_back([this]{
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_QueueMutex);
                    m_Condition.wait(lock,
                    [this]{ return m_Stop || !m_Tasks.empty();});
                    if(m_Stop && m_Tasks.empty())
                        return;
                    task = std::move(m_Tasks.front());
                    m_Tasks.pop();
                }
                task();
            }
        });
    }
    
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_Stop = true;
    }
    m_Condition.notify_all();
    for (auto &worker : m_Workers)
    {
        worker.join();
    }
}

} // namespace Rengin
