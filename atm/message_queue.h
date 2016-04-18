#ifndef message_queue_h
#define message_queue_h

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace messaging
{
    struct message_base
	{
	    virtual ~message_base() {}
	};

	template<typename Message>
	struct wrapped_messge: message_base
	{
	    Message contents;
		explicit wrapped_message(Message const& contents_):
			contents(contents_)
		{}
	};

	class queue
	{
	public:
		template<typename T>
		void push(T const& msg)
		{
		    std::lock_guard<std::mutex> lock(m);
			q.push(std::make_shared<wrapped_message<T>>(msg));
			c.notify_all();
		}

		std::shared_ptr<message_base> wait_and_pop()
		{
		    std::unique_lock<std::mutex> lock(m);
			c.wait(lock, [&]{return !q.empty();});
			auto res = q.front();
			q.pop();
			return res;
		}
	
	private:
		std::mutex m;
        std::condition_variable c;
		std::queue<std::shared_ptr<message_base>> q;
	};

}

#endif /* message_queue_h */

