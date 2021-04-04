#include "MySerializer.h"

void test_serializer_int() {
	int s = -500;
	int d;

	std::vector<char> buf;
	{
		MySerializer se(buf);
		se.io(s);
	}
	{
		MyDeserializer de(buf);
		de.io(d);
	}

	assert(s == d);
}

void test_serializer_string() {
	std::string s = "testing";
	std::string d;

	std::vector<char> buf;
	{
		MySerializer se(buf);
		se.io(s);
	}
	{
		MyDeserializer de(buf);
		de.io(d);
	}

	assert(s == d);
}

void test_serializer_vector() {
	std::vector<int> s = {1,2,3,4};
	std::vector<int> d;

	std::vector<char> buf;
	{
		MySerializer se(buf);
		se.io(s);
	}
	{
		MyDeserializer de(buf);
		de.io(d);
	}

	assert(s == d);
}

class MyTestData {
public:
	template<typename SE>
	void io(SE& se) {
		se.io(a);
		se.io(b);
	}

	bool operator==(const MyTestData& r) {
		return a == r.a
			&& b == r.b;
	}

	int16_t a = 0;
	int8_t  b = 0;
};

void test_serializer_MyTestData() {
	MyTestData s;
	s.a = 123;
	s.b = 45;

	MyTestData d;

	std::vector<char> buf;
	{
		MySerializer se(buf);
		se.io(s);
	}
	{
		MyDeserializer de(buf);
		de.io(d);
	}

	assert(s == d);
}

void test_serializer() {
	test_serializer_int();
	test_serializer_string();
	test_serializer_vector();
	test_serializer_MyTestData();
}
