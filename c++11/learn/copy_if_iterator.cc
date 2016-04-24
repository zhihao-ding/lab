#include <iostream>
#include <algorithm>
#include <vector>

template<typename C, typename F>
C filter(C& c, F&& f)
{
    C t(c.size());
	auto it = std::copy_if(c.begin(), c.end(), t.begin(), f);
	t.resize(std::distance(t.begin(), it));
	return t;
}

int main()
{
	std::vector<int> v{1,2,3,4,5,6,7,8,9,0};
	std::vector<int> v1(v.size());
	auto is_event = [](int i){ return i%2 == 0; };
    /*
	auto it = std::copy_if(v.begin(), v.end(), v1.begin(), is_event);
	v1.resize(std::distance(v1.begin(), it));
	for (auto i: v1)
	{
	    std::cout << "even: " << i << std::endl;
	}
	*/
	for (auto i: filter(v, is_event))
	{
	    std::cout << "~ even: " << i << std::endl;
	}

	return 0;
}
