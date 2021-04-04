#include <stdio.h>
#include <memory>
#include <ostream>
#include <iostream>
#include <functional>
#include <vector>
#include <optional>
#include <string_view>
#include <any>

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
//#if MY_CPP_VERSION >= 201703L
	#define MY_CPP17 1
//#endif

#define my_dumpvar(E) std::cout << #E << " = " << E << "\n";
#define run_test(E) std::cout << "\n==== " << #E << " ====\n"; E;

//====================================

#if MY_CPP14

auto auto_return_type_func1(int a) {
	if (a > 0) {
		return 1.0;
	}
	return -1.0;
}

void test_auto_return_type() {
	my_dumpvar(auto_return_type_func1(1));
}

constexpr int constexpr_loop(int n) {
	int sum = 0;
	for (int i = 1; i <= n; i++) {
		sum += i;
	}
	return sum;
}

void test_constexpr2() {
	int v = 10;
	my_dumpvar(constexpr_loop(4));

	switch (v) {
		case constexpr_loop(4):
			std::cout << "works";
		default:
			std::cout << "fail";
	}
}

template<typename T> T my_PI = T(3.1416);

void test_variable_template() {
	my_dumpvar(my_PI<int>);
	my_dumpvar(my_PI<double>);
}

void test_binary_literal() {
	my_dumpvar(0b101);
}

void test_digit_separators() {
	my_dumpvar(1'234'567);
}

void call_lambda_int(std::function<int (int)> f) {
	my_dumpvar(f(10));
}

void call_lambda_float(std::function<float (float)> f) {
	my_dumpvar(f(11.1f));
}

void test_generic_lambda() {
	auto f = [](auto i){ return i * 2; };
	call_lambda_int(f);
	call_lambda_float(f);
}

void test_lambda_capture_expression() {
	auto f = [v = 1000](int i) {
		return v + i;
	};

	call_lambda_int(f);
}

[[deprecated]] // issue warning
void test_deprecated() {
	my_dumpvar("deprecated");
}

void test_tuple_addressing_via_type() {
	std::tuple<int, float, double> t(11, 2.2f, 3.3);
	my_dumpvar(std::get<int>(t));
	my_dumpvar(std::get<float>(t));
	my_dumpvar(std::get<double>(t));
}

#endif // MY_CPP14

#if MY_CPP17

void test_static_assert() {
	static_assert(true, "message"); // C++11
	static_assert(true); // new in C++17 without message
}

void test_auto_deduction_from_braced_init_list() {
	auto a = { 1, 2 };	// a is std::initializer_list<int>
	auto b = { 3 };		// b is std::initializer_list<int>
	auto i { 3 };		// i is int
	auto j = { 3, 4 };	// j is std::initializer_list<int>

	my_dumpvar(typeid(decltype(a)).name());
	my_dumpvar(typeid(decltype(b)).name());
	my_dumpvar(typeid(decltype(i)).name());
	my_dumpvar(typeid(decltype(j)).name());
}

//namespace MyLib::UI {
//namespace UI {
//}
//}

// Nested Namespace definitions
namespace MyLib::UI {
}

void test_fallthrough() {
	int i = 0;
	switch(i) {
		case 0:
			my_dumpvar("case 0");
			// gcc -Wimplicit-fallthrough
			//break; // forgot break
		case 1:
			my_dumpvar("case 1");
			[[fallthrough]]; // against the fallthrough warning
		case 2:
			my_dumpvar("case 1");
			break;
	}
}

[[nodiscard]]
int plus_one(int a) {
	a = a + 1;
	return a;
}

void test_nodiscard() {
	int a = 10;
	plus_one(a); // warning for nodiscard

	a = plus_one(a);
	my_dumpvar(a);
}

#define my_unused(X) (void)(X)

// enable warning level 4
void test_maybe_unused() {
	int a = 0; //warning unused
	
	int b = 0;
	my_unused(b);  // stop warning unused
	
	[[maybe_unused]] // stop warning unused
	int c = 10;
}

template<typename T>
void if_constexpr_func() {
	if constexpr (std::is_same<T, int>::value) {
		my_dumpvar("OK");
	} else {
		return 0; // no return for void, but if_constexpr (false) will skip this
	}
}

void test_if_constexpr() {
	if_constexpr_func<int>();
	//if_constexpr_func<float>(); error
}

struct Point {
	Point(int x_, int y_)
		: x(x_)
		, y(y_)
	{}
	int x, y;
};

Point getPoint() {
	return Point(1, 2);
}

std::tuple<int, int> getTuple2() {
	return {11, 22};
}

void test_structured_binding() {
	auto [a,b] = getPoint();
	my_dumpvar(a);
	my_dumpvar(b);

	auto [c,d] = getTuple2();
	my_dumpvar(c);
	my_dumpvar(d);
}

struct MyObject {
	// non-copyable
	MyObject(const MyObject&) = delete;
	void operator=(const MyObject&) = delete;

	MyObject(int v_) : v(v_) {}

	int v = 0;
};

void test_initializer_in_if() {
	std::shared_ptr<MyObject> obj(new MyObject(100));
	std::weak_ptr<MyObject> wp(obj);

	{
		auto sp = wp.lock();
		if (sp) {
			my_dumpvar(sp->v);
		}
	}

	// put 'sp' inside 'if' scope
	if (auto sp = wp.lock()) {
		my_dumpvar(sp->v);
	}
}

template<typename T>
class  MyTData {
public:
	MyTData(T v_) : v(v_) {}
	T v;
};

template<typename T>
MyTData<T> make_MyTData(T i) {
	return MyTData<T>(i);
}

void test_constructor_template_deduction() {
	auto a = MyTData<int>(10);
	auto b = make_MyTData(10);

//	auto c = MyTData(10);  // doesn't support in Visual C++ yet, but gcc does

	my_dumpvar(a.v);
	my_dumpvar(b.v);
}

inline
std::ostream& operator << (std::ostream& s, const std::string_view& v) {
	for (auto& c : v) {
		s << c;
	}
	return s;
}

void test_string_view() {
	auto s = std::string("ABCDE"); // same as string("ABCDE")

	std::string_view a("1234");
	std::string_view b(s.data(), 2);

	my_dumpvar(a);
	my_dumpvar(a.size());

	my_dumpvar(b);
	my_dumpvar(b.size());
}

template<typename T>
std::ostream& operator << (std::ostream& s, const std::optional<T>& v) {
	if (v) {
		s << v.value();
	} else {
		s << "nullopt";
	}
	return s;
}

void test_optional() {
	std::optional<int> a = 123;
	std::optional<int> b = 345;

	my_dumpvar(a);
	my_dumpvar(b);

	a.reset();
	b = std::nullopt;
	my_dumpvar(a);
	my_dumpvar(b);
}

std::ostream& operator << (std::ostream& s, const std::any& v) {
	auto& t = v.type();
	s << "(" << t.name() << ") ";

	if (t == typeid(int)) {
		s << std::any_cast<int>(v);

	} else if (t == typeid(double)) { 
		s << std::any_cast<double>(v);

	} else if (t == typeid(const char*)) { 
		s << std::any_cast<const char*>(v);

	} else if (t == typeid(std::string)) { 
		s << std::any_cast<const std::string&>(v).c_str();

	} else if (t == typeid(void)) {
		s << "void";

	} else {
		s << "[unsupported type " << t.name() << "]";
	}
	return s;
}

void test_any() {
	std::any a;
	my_dumpvar(a);

	a = 1;
	my_dumpvar(a);

	a = 2.3;
	my_dumpvar(a);

	a = "ABC";
	my_dumpvar(a);

	a = std::string("Hello");
	my_dumpvar(a);

	a.reset();
	my_dumpvar(a);
}

#endif // MY_CPP_VERSION >= 201703L

int main() {
	my_dumpvar(MY_CPP_VERSION);

#if MY_CPP14
	run_test(test_auto_return_type());
	run_test(test_constexpr2());
	run_test(test_variable_template());
	run_test(test_binary_literal());
	run_test(test_digit_separators());
	run_test(test_generic_lambda());
	run_test(test_lambda_capture_expression());
	run_test(test_deprecated());
	run_test(test_tuple_addressing_via_type());
#endif

#if MY_CPP17
	run_test(test_static_assert());
	run_test(test_auto_deduction_from_braced_init_list());
	run_test(test_fallthrough());
	run_test(test_nodiscard());
	run_test(test_maybe_unused());
	run_test(test_if_constexpr());
	run_test(test_structured_binding());
	run_test(test_initializer_in_if());
	run_test(test_constructor_template_deduction());
	run_test(test_string_view());
	run_test(test_optional());
	run_test(test_any());
#endif

	std::cout << "\n==== Program Ended ====\n";
#ifdef _MSC_VER
	std::cout << "press any key to exit !!\n";
	_getch();
#endif

	return 0;
}