#ifndef join_threads_h
#define join_threads_h

class join_threads
{
public:
	explicit join_threads(std::vector<std::thread>& threads_):
		threads(threads_)
	{}

	~join_threads()
	{
	    for (std::thread& t: threads)
		{
		    if (t.joinable()) t.join();
		}
	}

private:
	std::vector<std::thread> &threads;
};

#endif /* join_threads_h */
