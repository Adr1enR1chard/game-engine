#pragma once
#include <functional>
#include <mutex>
#include <vector>

class ThreadQueue
{
public:
    ThreadQueue() = default;
    ~ThreadQueue() = default;

    ThreadQueue(const ThreadQueue &) = delete;
    ThreadQueue &operator=(const ThreadQueue &) = delete;

    /// @brief Enqueue a new task to be executed later.
    /// @param task The task to enqueue.
    void enqueue(const std::function<void()> &task)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.push_back(task);
    }

    /// @brief Execute all tasks in the queue. This should be called from the main thread.
    void executeAll()
    {
        // Copy tasks to avoid holding the lock while executing
        // Other threads can enqueue new tasks while we are executing
        std::vector<std::function<void()>> tasksCopy;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            tasksCopy.swap(m_tasks);
        }

        for (const auto &task : tasksCopy)
        {
            task();
        }
    }

private:
    std::vector<std::function<void()>> m_tasks;
    std::mutex m_mutex;
};