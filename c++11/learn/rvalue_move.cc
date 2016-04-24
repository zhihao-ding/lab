#include <iostream>
#include <vector>

class A
{
public:
	A(): m_ptr(new int(0)) {
	    std::cout << "A()" << std::endl;
	}

	A(const A &a): m_ptr(new int(*a.m_ptr)) {
	    std::cout << "A(const A&)" << std::endl;
	}

	A(A&& a): m_ptr(a.m_ptr) {
		std::cout << "A(A&&)" << std::endl;
	    a.m_ptr = nullptr;
	}

	~A() {
	    std::cout << "~A()" << std::endl;
		delete m_ptr; 
	}
private:
	int *m_ptr;
};

A get_a(bool flag)
{
    A a, b;
	std::cout << "get_a ..." << std::endl;
	if (flag) {
	    return a;
	} else {
	    return b;
	}
}


class MyString
{
public:
	MyString():
		m_data(nullptr), m_len(0)
	{}

	explicit MyString(const char *p) {
	    m_len = strlen(p);
		copy_data(p);
		std::cout << "constructor: " << p << std::endl;
	}

	MyString(const MyString &str) {
	    m_len = str.m_len;
		copy_data(str.m_data);
		std::cout << "copy_constructor: " << str.m_data << std::endl;
	}

	MyString(MyString&& str):
	    m_data(str.m_data), m_len(str.m_len)
	{
	    str.m_data = nullptr;
		std::cout << "move_constructor: " << m_data << std::endl;
	}

	MyString& operator=(const MyString& str) {
	    if (this != &str) {
			delete [] m_data;
		    m_len = str.m_len;
			copy_data(str.m_data);
		}
		std::cout << "copy_assignment_constructor: " << str.m_data << std::endl;
		return *this;
	}

	MyString& operator=(MyString&& str) {
	    if (this != &str) {
			delete [] m_data;
		    m_len = str.m_len;
			m_data = str.m_data;
			str.m_data = nullptr;
		}
		std::cout << "mov_assignment_constructor: " << m_data << std::endl;
		return *this;
	}

	~MyString() {
	    delete [] m_data;
	}

	void print() {
		if (m_data) {
	        std::cout << "print: " << m_data << ", at: " << &m_data << std::endl;
		} else {
		    std::cout << "print nullptr" << std::endl;
		}
	}

private:
	void copy_data(const char* s) {
	    m_data = new char[m_len + 1];
		memcpy(m_data, s, m_len);
		m_data[m_len] = '\0';
	}

private:
	char *m_data;
	size_t m_len;
};

void test()
{
    MyString a = MyString("Hello");
	std::vector<MyString> vec;
	vec.push_back(std::move(a));
	MyString&& b = std::move(a);
	MyString&& c = std::move(a);
	a.print();
	b.print();
	c.print();
}

int main()
{
	// A a = get_a(false);
	//int *p = nullptr;
	//delete [] p;
	test();
    return 0;
}

