#include "MyVector.h"
#include "MyArray.h"
#include <conio.h>
#include <stdio.h>
#include <vector>

class MyColor {
	float r,g,b,a;
};

template<>
struct isRawCopyable<MyColor> {
	static const bool value = true;
};

class MyData : public NonCopyable {
public:
	MyData(int t) : test(t) {}

	MyData(MyData && r) 
		: test(r.test)
	{
		r.test = 0;
	}

	void print() const {
		printf("MyData %d\n", test);
	}

private:
	int test = 0;
};

void dump(const MyIVector<int>& v) {
	printf("MyVector<int> size = %d, cap = %d\n", v.size(), v.capacity());
//	for (int i = 0; i < v.size(); i++) {
//		printf(" %d", v[i]);
//	}

//	for (auto it = v.begin(); it != v.end(); ++it) {
//		auto& e = *it;
//	}

	for (auto& e : v) {
		printf(" %d", e);
	}
	printf("\n");

//	for (auto it = v.rbegin(); it != v.rend(); ++it) {
//		auto& e = *it;
//		printf(" %d", e);
//	}

	for (auto& e : v.revEach()) {
		printf(" %d", e);
	}
	printf("\n");

	printf("\n");
}

void dump(const MyIVector<MyData> & v) {
	printf("MyVector<MyData> size = %d, cap = %d\n", v.size(), v.capacity());
	for (int i = 0; i < v.size(); i++) {
		v[i].print();
	}
	printf("\n");
}

void test001() {
	MyVector<int> v;

	for (int i = 0; i < 10; i++) {
		v.append(i + 10);
	}
	dump(v);
}

void test002() {
	MyData t(10);
	MyData a(std::move(t));  // <--- NonCopyable

	MyVector<MyData> v;
	v.append(std::move(t));
	v.append(std::move(a));
	v.append(MyData(99));

	v.emplace_back(std::move(a));
	v.emplace_back(77);

	dump(v);
}

void test003() {
	std::vector<int> a;
	a.resize(10);
	a.clear();
	a.shrink_to_fit();
}

void test004() {
	MyVector<int, 0> v;
	v.resize(1);
	v.resize(4);
	v.resize(16);

	int i = 0;
	for (auto& e : v) {
		e = i;
		i++;
	}

	dump(v);

//	String <-- array of char + null terminator
//	MyString<char, 10> s;  
//	MyString<wchar_t, 4> w;
}

void FillData(MyVector<int> & v) {
	for (auto& e : v) {
		e = 1;
	}
}

int main() {
	test004();

	printf("\n====== Program Ended =========\n");
	_getch();
	return 0;
}