#include <iostream>
//#include <thread>
//#include <utility>
#include <future>
#include <chrono>

int main()
{
	std::future<int> f1 = std::async(std::launch::async, [](){ return 8; });
	std::cout << f1.get() << std::endl;

	std::future<int> f2 = std::async(std::launch::async, [](){
	    std::cout << "[" << std::this_thread::get_id() << "] out: 8" << std::endl;
		return 0;
	});

    f2.wait();

	std::future<int> future = std::async(std::launch::async, [](){
	    std::this_thread::sleep_for(std::chrono::seconds(3));
		return 8;
	});

	std::cout << "waiting start ..." << std::endl;

	std::future_status status;
	do
	{
	    status = future.wait_for(std::chrono::seconds(1));
		if (std::future_status::deferred == status) {
		    std::cout << "~ deferred" << std::endl;
		} else if (std::future_status::timeout == status) {
		    std::cout << "~ timeout" << std::endl;
		} else if (std::future_status::ready == status) {
		    std::cout << "~ ready" << std::endl;
		} else {
		    std::cout << "ERROR: Unknown status!" << std::endl;
		}
	} while(status != std::future_status::ready);

	std::cout << "result is: " << future.get() << std::endl;
    return 0;
}

