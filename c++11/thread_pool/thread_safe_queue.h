#ifndef thread_safe_queue_h
#define thread_safe_queue_h

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class thread_safe_queue
{
pubilc:
	thread_safe_queue() {}

	void push(T new_value)
	{
		std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
	    std::lock_guard<std::mutex> lock(m);
	    q.push(data);
	    c.notify_one();
	}

	void wait_and_top(T& value)
	{
		std::unique_lock<std::mutex> lock(m);
		c.wait(lock, [this]{ return !q.empty(); });
		value = std::move(*q.front());
		q.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
	    std::unique_lock<std::mutex> lock(m);
		c.wait(lock, [this]{ return !q.empty(); });
 		std::shared_ptr<T> res = q.front();
		q.pop();
		return res;
	}

	bool try_pop(T& value)
	{
	    std::lock_guard<std::mutex> lock(m);
		if (q.empty()) return false;
		value = std::move(*q.front());
		q.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
	    std::lock_guard<std::mutex> lock(m);
		if (q.empty()) return nullptr; //std::shared_ptr<T>();
		std::shared_ptr<T> res = q.front();
		q.pop();
		return res;
	}

	bool empty() const
	{
	    std::lock_guard<std::mutex> lock(m);
		return q.empty();
	}

private:
	std::queue<std::shared_ptr<T>> q;
	mutable std::mutex m;
	std::condition_variable c;
};

#endif /* thread_safe_queue_h */

