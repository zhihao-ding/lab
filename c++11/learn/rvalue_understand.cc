#include <iostream>

int g_construct = 0;
int g_copy_construct = 0;
int g_destruct = 0;

class A 
{
public:
	A() {
	    std::cout << "[construct] - " 
			      << ++g_construct << std::endl; 
	}

	A(const A &a) {
	    std::cout << "[copy_construct] - "
			      << ++g_copy_construct << std::endl;
	}

	~A() {
	    std::cout << "[destruct] - "
			      << ++g_destruct << std::endl;
	}

	void print() {
	    std::cout << "print ..." << std::endl;
	}
};


A get_a() {
    return A();
}

int main()
{
    //A a = get_a();
	//A &b = get_a();
	const A &b = get_a();
    //b.print();
	//A &&a = get_a();
	//a.print();
    return 0;
}


