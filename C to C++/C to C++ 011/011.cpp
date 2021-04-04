#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>
#include <stdlib.h>
#include <new>
#include <vector>

class NonCopyable {
public:
	NonCopyable() {}
private:
	NonCopyable(const NonCopyable&);
	void operator=(const NonCopyable&);
};

class MyObject : public NonCopyable {
public:
	void func() {
		printf("Hi\n");
	}
};

template<class T>
class AutoPtr : public NonCopyable {
public:
	AutoPtr(T* p = nullptr) 
		: m_p(p)
	{
	}

	// move constructor
	AutoPtr(AutoPtr && rhs) {
		m_p = rhs.m_p;
		rhs.m_p = nullptr;
	}

	//take ownership
	void operator=(AutoPtr && rhs) {
		delete m_p;
		m_p = rhs.m_p;
		rhs.m_p = nullptr;
	}

	~AutoPtr() { delete m_p; }

	T* ptr() { return m_p; }
	T* operator->() { return m_p; }

	void reset(T* p) {
		delete m_p;
		m_p = p;
	}

private:
	T* m_p = nullptr;
};

void auto_example() {
	//int i = 10;
	auto i = 10;

	//float f = 5.5f;
	auto f = 5.5;

	//int* p = &i;
	//auto  p = &i; // OK
	auto* p = &i; // also OK!!

	//int& r = i;
	auto& r = i;

	const auto& c = i;

	int* np = nullptr;
}

template<class T>
class MyArray {
public:
	void resize(size_t req_size) {

	}

	void reserve(size_t req_size) {
		if (req_size <= m_capacity) {
			return;
		}

		T* new_data = reinterpret_cast<T*>( malloc(sizeof(T) * req_size) );
		for (size_t i = 0; i < m_size; i++) {
			//new_data[i] = m_data[i];

			T* p = &new_data[i];

			//placement new
			//new(p) T(m_data[i]); //copy constructor <-- not working for non-copyable

			T& o = m_data[i];
			//placement new + move semantic
			new(p) T( std::move(o)); // T& -> T&&

			m_data[i].~T();
		}

		free(m_data);
		m_data = new_data;
		m_capacity = req_size;
	}

private:
	T* m_data = nullptr;
	size_t m_size = 0;
	size_t m_capacity = 0;
};

int main() {
	MyArray< AutoPtr<MyObject> > a;
	a.reserve(10);

	AutoPtr<MyObject> p0;
	AutoPtr<MyObject> p1;

	MyArray<MyObject> b;
	b.reserve(10);

	//std::vector<MyObject> c; // <---


	//p0.take(p1)
	p0 = std::move(p1); // move semantics

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}