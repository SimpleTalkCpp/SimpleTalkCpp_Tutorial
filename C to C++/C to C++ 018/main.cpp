#include <stdio.h>
#include <memory>
#include <ostream>
#include <iostream>
#include <functional>
#include <vector>

#ifdef _MSC_VER
	#include <conio.h>
	#define MY_CPP_VERSION _MSVC_LANG
#else
	#define MY_CPP_VERSION __cplusplus
#endif

#if MY_CPP_VERSION >= 201103L
	#define MY_CPP11 1
#endif

// C++ 14 - require /std:c++14
#if MY_CPP_VERSION >= 201402L
	#define MY_CPP14 1
#endif

// C++17 - require /std:c++17
#if MY_CPP_VERSION >= 201703L
	#define MY_CPP17 1
#endif

#define my_dumpvar(E) std::cout << #E << " = " << E << "\n";
#define run_test(E) std::cout << "\n==== " << #E << " ====\n"; E;

//====================================

#if MY_CPP11

class alignas(8) Point {
public:
	Point() = default;

	explicit Point(int x_, int y_) 
	: x(x_)
	, y(y_)
	{
		std::cout << "call Point(int, int)\n";
	}

	// delegation constructor
	Point(int s) : Point(s,s) {
		std::cout << "call Point(int)\n";
	}

	Point(std::initializer_list<int> list) {
		std::cout << "call Point(intitializer_list)\n";
		for (auto& v : list) {
			x += v;
		}
	}

	int x = 0;
	int y = 0;
};

class alignas(Point) PointExample2 {
};

class Point3D : public Point {
public:
	int z = 0;
};

static_assert(sizeof(Point) == sizeof(int) * 2, "some error message");
static_assert(std::is_same<decltype(Point::x), int>::value, "Point x must be int");
static_assert(std::is_base_of<Point, Point3D>::value, "Point3D must based on Point");

std::ostream& operator << (std::ostream& s, const Point& v) {
	s << "Point(" << v.x << ", " << v.y << ")";
	return s;
}

class UniquePtr {
public:
	~UniquePtr() { destroy(); }

	UniquePtr() = default;

	UniquePtr(Point* p)
		: m_p(p) 
	{
		std::cout << "call UniquePtr(Point*)\n";
	}

	UniquePtr(nullptr_t) {
		std::cout << "call UniquePtr(nullptr_t)\n";
	}

	UniquePtr(int v) {
		std::cout << "call UniquePtr(int)\n";
	}

	// copy constructor
	UniquePtr(const UniquePtr&) = delete;

	// move constructor
	UniquePtr(UniquePtr && r) {
		operator=(std::move(r));
	}

	// copy operator
	void operator=(const UniquePtr&) = delete;

	// move operator
	void operator=(UniquePtr && r) {
		destroy();
		m_p = r.m_p;
		r.m_p = nullptr;
	}

	void create(int x, int y) {
		destroy();
		m_p = new Point(x, y);
	}
	
	void destroy() {
		if (m_p) {
			delete m_p;
			m_p = nullptr;
		}
	}

	explicit operator bool() const {
		return m_p != nullptr;
	}

		  Point* getPtr()		{ return m_p; }
	const Point* getPtr() const { return m_p; }

private:
	Point* m_p = nullptr;
};

std::ostream& operator << (std::ostream& s, const UniquePtr& v) {
	auto* p = v.getPtr();
	if (!p) {
		s << "(null)";
	} else {
		s << *p;
	}
	return s;
}

void test_auto() {
	int  a = 10;
	auto b = 10;
	decltype(a) d = 11; // int d = 11;

	auto  p0 = &a;
	auto* p1 = &a;

	auto& r = a;
	const auto& cr = a;

	my_dumpvar(a);
	my_dumpvar(b);
	my_dumpvar(d);
	my_dumpvar(p0);
	my_dumpvar(p1);
	my_dumpvar(r);
	my_dumpvar(cr);
}

void test_move() {
	UniquePtr a;
	a.create(3,4);

	// copy
//	UniquePtr c = a;
//	UniquePtr d(a);

	UniquePtr b;
	b = std::move(a); // a --> &&a

	my_dumpvar(a);
	my_dumpvar(b);

	UniquePtr c(std::move(b));

	my_dumpvar(a);
	my_dumpvar(b);
	my_dumpvar(c);
}

constexpr int div2(int m) {
	return m / 2;
}

void test_constexpr() {
	char a[div2(4)];
	my_dumpvar(sizeof(a));
}

void test_range_based_for_loop() {
	std::vector<int> v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i);
	}

	// C++ 03
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";

	// auto
	for (auto& it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";

	// range based for loop
	for (const auto& e : v) {
		std::cout << e << " ";
	}
	std::cout << "\n";
}

void test_initializer_list() {
	Point a = {1,2,3,4};
	my_dumpvar(a);
}

void call_lambda(std::function<int (int)> f) {
	for (int i = 0; i < 4; i++) {
		auto ret = f(i);
		std::cout << "call func i=" << i << ", ret=" << ret << "\n";
	}
}

void test_lambda() {
	int sum = 0;
	call_lambda([&sum](int i) -> int {
		sum += i;
		return i * 2;
	});
	my_dumpvar(sum);
}

void test_delegation_constructor() {
	Point p(3);
	my_dumpvar(p);
}

void test_nullptr_t() {
	UniquePtr a(NULL);          // <-- call UniquePtr(int)  !!! unexpected
	UniquePtr b(new Point());   // <-- call UniquePtr(Point*)
	UniquePtr c(nullptr);       // <-- call UniquePtr(nullptr_t)
}

enum Test {
	Test_A,
	Test_B,
	Test_C,
};

enum class TestEnum : int8_t {
	A,
	B,
	C,
};

template<typename T> inline
typename std::underlying_type<T>::type my_enum_to_int(T v) {
	return static_cast<std::underlying_type<T>::type>(v);
}

void test_enum_class() {
	int a = Test_A;

	TestEnum e = TestEnum::C;
	my_dumpvar(sizeof(e));

	auto i = my_enum_to_int(e);
	printf("i = %d", i);

	my_dumpvar((std::is_same<TestEnum, int8_t>::value));

	typedef std::underlying_type<TestEnum>::type Under;
	my_dumpvar((std::is_same<Under,    int8_t>::value));
}

class Data {
public:
	explicit Data(int i) : value(i) {}
	int value;
};

void func_data(const Data& data) {
	my_dumpvar(data.value);
}

void test_explicit_constructor() {
	func_data(Data(10));
	// func_data(11); // compile error after added explicit
}

void test_explicit_operator_bool() {
	UniquePtr p(new Point(1,2));
	if (p) {
		printf("p is not null");
	}
}

template<typename FIRST, typename... ARGS>
void my_print_impl(const FIRST& first, const ARGS&... args) {
	std::cout << first << " ";
	my_print_impl(args...);
}

void my_print_impl() {
}

template<typename... ARGS>
void my_print(const ARGS&... args) {
	my_dumpvar(sizeof...(args));
	my_print_impl(args...);
}

void test_variadic_templates() {
	my_print (1, 2, 3, "a", "b");
}

namespace NS_A {
	int i;
}

namespace NS_B {
	int i;
}

void using_example1() {
	using namespace NS_A;
	using namespace NS_B;
	
	using NS_B::i;
	int a = i;
}

//typedef int MyInt;
using MyInt = int; // 'using' instead typedef

//typedef void (*MyCallback)(int a);
using MyCallback = int (int a);
using MyCallback1 = auto (int a) -> int; // Alternative function syntax

// no template typedef !!!!
//template<typename T> typedef std::vector< std::unique_ptr<T> > UPtrVector;
// ! new feature
template<typename T> using UPtrVector = std::vector< std::unique_ptr<T> >;

void test_template_using() {
	UPtrVector<int> a;
}

void test_string_literal() {
	const char     a[] = "abcd";
	const char     b[] = u8"abcd";
	const char16_t c[] = u"abcd";
	const char32_t d[] = U"abcd";
	const wchar_t  e[] = L"abcd";
}

std::tuple<int, int, float> tuple_example() {
	return {1, 2, 3.1f};
}

void test_tuple() {
	auto t = tuple_example();
	my_dumpvar(std::get<0>(t));
	my_dumpvar(std::get<1>(t));
	my_dumpvar(std::get<2>(t));
}

#endif // MY_CPP11

int main(int argv, const char* argc[]) {
	my_dumpvar(MY_CPP_VERSION);

#if MY_CPP11
	run_test(test_auto());
	run_test(test_move());
	run_test(test_constexpr());
	run_test(test_range_based_for_loop());
	run_test(test_initializer_list());
	run_test(test_explicit_constructor());
	run_test(test_lambda());
	run_test(test_delegation_constructor());
	run_test(test_nullptr_t());
	run_test(test_enum_class());
	run_test(test_variadic_templates());
	run_test(test_template_using());
	run_test(test_explicit_operator_bool());
	run_test(test_string_literal());
	run_test(test_tuple());
#endif

	std::cout << "\n==== Program Ended ====\n";
#ifdef _MSC_VER
	std::cout << "press any key to exit !!\n";
	_getch();
#endif
	return 0;
}