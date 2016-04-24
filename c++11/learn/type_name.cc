#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <memory>
#include <string>
#include <cstdlib>

template<typename T>
std::string type_name()
{
    using TR = typename std::remove_reference<T>::type;
    std::string r = typeid(TR).name();
	if (std::is_const<TR>::value) {
	    r += " const";
	}
	if (std::is_volatile<TR>::value) {
	    r += " volatile";
	}
	if (std::is_lvalue_reference<T>::value) {
	    r += " lvalue";
	} else if (std::is_rvalue_reference<T>::value) {
	    r += " rvalue";
	}
	return r;
}

template<typename T>
void func(T&& t) {
    std::cout << type_name<T>() << std::endl;
}

int main()
{
    std::string str = "test";
	func(str);
	func(std::move(str));
	func("test");
	func(0);
	return 0;
}
