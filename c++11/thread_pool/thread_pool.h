#ifndef thread_pool.h
#define thread_pool.h

#include <thread>
#include <vector>

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
	if (!length) return init;

	unsigned long const block_size = 25;
	unsigned long const num_blocks = (length + block_size - 1) / block_size;

	std::vector<std::future<T>> futures(num_blocks - 1);
	thread_pool pool;

	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_blocks - 1); ++i)
	{
	    Iterator block_end = block_start;
		std::advance(block_end, block_size);
		futures[i] = pool.sumbit(accumulate_block<Iterator, T>());
		block_start = block_end;
	}

	T last_result = accumulate_block<Iterator, T>()(block_start, last);
	T result = init;
	for (unsigned long i = 0; i < (num_blocks - 1); ++i)
	{
	    result += futures[i].get();
	}
	result += last_result;
	return result;
}

class function_wrapper
{
public:
	template<typename F>
	function_wrapper(F&& f):
		impl(new impl_type<F>(std::move(f)))
	{}

	void operator()() { impl->call(); }

	function_wrapper() = default;

	function_wrapper(function_wrapper&& other):
		impl(std::move(other.impl))
	{}

	function_wrapper& operator=(function_wrapper&& other)
	{
	    impl = std::move(other.impl);
		return *this;
	}

	function_wrapper(const function_wrapper&)=delete;
	function_wrapper(function_wrapper&)=delete;
	function_wrapper& operator=(const function_wrapper&)=delete;

private:
	struct impl_base {
	    virtual void call() = 0;
		virtual ~impl_base() {}
	};

	std::unique_ptr<impl_base> impl;
	template<typename F>
	struct impl_type: impl_base
	{
	    F f;
		impl_type(F&& f_): f(std::move(f_)) {}
		void call() { f(); }
	};
};


class thread_pool
{
public:
	thread_pool():
		done(false), joiner(threads)
	{
	    unsigned const thread_count = std::thread::hardware_concurrency();

		try
		{
			for (unsigned i = 0; i < thread_count; ++i)
			{
			    threads.push_back(std::thread(&thread_pool::worker_thread, this));
			}
		}
		catch (...)
		{
			done = true;
			throw;
		}
	}

	~thread_pool()
	{
	    done = true;
	}

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type>
	submit(FunctionType f)
	{
		using result_type = typename std::result_of(FunctionType())::type;
		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
	}

private:
	void worker_thread()
	{
		while(!done)
		{
		    function_wrapper task;
			if (work_queue.try_pop(task))
			{
			    task();
			}
			else
			{
			    std::this_thread::yield();
			}
		}
	}

private:
	std::atomic_bool done;
	thread_safe_queue<function_wrapper> work_queue;
	std::vector<std::thread> threads;
	join_threads joiner;
};

#endif /* thread_pool.h */

