#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>
#include <stdlib.h>
#include <memory>
#include <vector>

//STL standard template library

class NonCopyable {
public:
	NonCopyable() {}
private:
	NonCopyable(const NonCopyable&);
	void operator=(const NonCopyable&);
};

class MyObject : public NonCopyable {
public:
	//vector resize require, constructor without parameter
	MyObject() {
	}

	//vector emplace_back example
	MyObject(int value) {
		m_value = value;
	}

	//vector reserve or resize require move contructor to move elements to new buffer
	MyObject(MyObject && r) {
		m_value = r.m_value;
		r.m_value = 0;
	}

	void func() {
		printf("Hi\n");
	}
private:
	int m_value = 0;
};

template<class T>
inline std::unique_ptr<T> make_unique_ptr(T* p) {
	return std::unique_ptr<T>(p);
}

void vector_example() {
	std::vector<MyObject> arr;
	arr.resize(10);
	arr.clear(); // same as arr.resize(0);

	// before c++11
	{
		//free memory by swap
		std::vector<MyObject> tmp;
		tmp.swap(arr);
	}
	// c++ 11
	arr.shrink_to_fit();
}

int main() {
	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}