#ifndef _threadsafe_queue_h_
#define _threadsafe_queue_h_

#include <mutex>
#include <queue>
#include <condition_variable>
#include <memory>
#include <thread>

template<typename T>
class threadsafe_queue
{
public:
    threadsafe_queue() {}

    void push(T new_value) 
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        data_queue.push(std::move(new_value));
        queue_condition.notify_one(); // notify one
    }

    void wait_and_pop(T& value) 
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_condition.wait(lock, [this]{return !data_queue.empty();});
        value = std::move(data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_condition.wait(lock, [this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (data_queue.empty())
            return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        return data_queue.empty();
    }

private:
    mutable std::mutex queue_mutex;
    std::queue<T> data_queue;
    std::condition_variable queue_condition;
};

#endif // _threadsafe_queue_h_
