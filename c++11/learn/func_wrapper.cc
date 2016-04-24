#include <iostream>
#include <string>

template<class Function, class... Args>
inline auto FuncWrapper(Function&& f, Args&& ... args)
	-> decltype(f(std::forward<Args>(args)...))
{
    return f(std::forward<Args>(args)...);
}

void test0()
{
    std::cout << "void" << std::endl;
}

int test1()
{
    return 1;
}

int test2(int x)
{
    return x;
}

std::string test3(std::string s1, std::string s2)
{
    return s1 + s2;
}

int main()
{
	FuncWrapper(test0);
	std::cout << FuncWrapper(test1) << std::endl;
	std::cout << FuncWrapper(test2, 3) << std::endl;
	std::cout << FuncWrapper(test3, "Hello", "Word!") << std::endl;
    return 0;
}

