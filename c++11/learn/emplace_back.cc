#include <iostream>
#include <vector>
#include <map>
#include <string>

struct A
{
    int x;
	double y;
	A(int a, double b): x(a), y(b) {}
};

class Complicated
{
public:
	Complicated(int a_, double b_, std::string c_)
		: a(a_), b(b_), c(c_)
	{
	    std::cout << "is constructed" << std::endl;
	}

	Complicated(const Complicated& other)
		: a(other.a), b(other.b), c(std::move(other.c))
	{
	    std::cout << "is moved" << std::endl;
	}

private:
	int a;
	double b;
	std::string c;
};

void test_complicated()
{
    std::map<int, Complicated> m;

	int a = 2;
	double b = 4.0;
	std::string c = "c++";

	std::cout << "-- insert to map..." << std::endl;
	m.insert(std::make_pair(3, Complicated(a, b, c)));

	std::cout << "-- emplace to map..." << std::endl;
	m.emplace(4, Complicated(a, b, c));

	std::vector<Complicated> v;
    std::cout << "-- emplace to vector..." << std::endl;
	v.emplace_back(a, b, c);

	std::cout << "-- push to vector..." << std::endl;
	v.push_back(Complicated(a, b, c));
}

int main()
{
    //vector<A> v;
	//v.emplace_back(1, 2.0);
	//cout << v.size() << endl;
	test_complicated();
	return 0;
}
