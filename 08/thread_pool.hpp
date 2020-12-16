#pragma once

#include <future>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <condition_variable>

class ThreadPool
{
    std::vector<std::thread> pool_;
    std::queue<std::function<void()>> taskQueue_;
    std::condition_variable sleeping_;
    std::mutex event_;
    bool stopExecuting_;
public:
    explicit ThreadPool(size_t poolSize  = std::thread::hardware_concurrency());

    ~ThreadPool();

     // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>;
};

ThreadPool::ThreadPool(size_t poolSize): stopExecuting_(false)
{
    pool_.reserve(poolSize);

    for (size_t i = 0; i < poolSize; i++)
    {
        pool_.emplace_back(
        [&]
        {
            for (;;)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(event_);
                    sleeping_.wait(lock, [&] { return !taskQueue_.empty() || stopExecuting_; } );

                    if (stopExecuting_)
                        break;

                    task = std::move(taskQueue_.front());
                    taskQueue_.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(event_);
        stopExecuting_ = true;
    }

    sleeping_.notify_all();

    for (auto& thread : pool_)
        thread.join();     
}

template <class Func, class... Args>
auto ThreadPool::exec(Func func, Args... args) -> std::future<decltype(func(args...))>
{
    auto wrapper = std::make_shared<std::packaged_task<decltype(func(args...)) ()>>(std::bind(func, args...));


    {
        std::unique_lock<std::mutex> lock(event_);
        taskQueue_.emplace(
        [=]
        {
            (*wrapper)();
        });
    }

    sleeping_.notify_one();

    return wrapper->get_future();
}
