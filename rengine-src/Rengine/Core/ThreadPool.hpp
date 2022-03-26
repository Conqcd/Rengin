#pragma once
#include "core.hpp"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <memory>
#include <future>

namespace Rengin
{
class ThreadPool
{
private:
    std::vector<std::thread> m_Workers;
    std::queue<std::function<void()>> m_Tasks;

    std::mutex m_QueueMutex;
    std::condition_variable m_Condition;
    bool m_Stop;
public:
    ThreadPool(int size = 4);
    ~ThreadPool();

    template<class Function,class... Args>
    auto enqueue(Function&& f,Args&&... args)
        -> std::future<typename std::result_of<Function>::type>
    {
        using return_type = typename std::result_of<Function(Args...)>::type;

        auto task = CreateRef<std::packaged_task<return_type()>>(std::bind(std::forward<Function>(f),std::forward<Args>(args)...));

        auto res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);

            if(m_Stop)
                throw std::runtime_error("Can not enqueue a task after the pool stop");
            
            m_Tasks.emplace([task]() { (*task)();});
        }
        m_Condition.notify_one();
    }
};


} // namespace Rengin
