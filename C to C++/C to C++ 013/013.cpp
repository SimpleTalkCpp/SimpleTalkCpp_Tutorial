#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>

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

int main() {
	std::list<int> ls;
	std::vector<int> v;

	for (int i = 0; i < 10; i++) {
		ls.push_back(i);
		v.push_back(i);
	}

	for (auto it = ls.begin(); it != ls.end(); ++it ) {
		int value = *it;
		printf("%d ", value);
	}
	printf("\n");

	for (auto it = v.begin(); it != v.end(); ++it ) {
		int value = *it;
		printf("%d ", value);
	}
	printf("\n");

	// find and erase
	auto q = std::find( v.begin(), v.end(), 4 );
	if (q == v.end()) {
		printf("not found");
	}else{
		v.erase(q);
	}

	for (auto it = v.begin(); it != v.end(); ) {
		int value = *it;

		if (value % 2) {
			it = v.erase(it);
		} else {
			++it;
		}
	}
	printf("\n");

	// range for loop
	for (auto& value : v) {
		printf("%d ", value);
	}
	printf("\n");

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}