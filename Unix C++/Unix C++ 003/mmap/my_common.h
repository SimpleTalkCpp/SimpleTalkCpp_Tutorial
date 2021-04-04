#include <exception>
#include <iostream>

class NonCopyable {
public:
	NonCopyable() {}

private:
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

class MyError : public std::exception {
public:
	MyError(const char* msg) {
		printf("MyError(%s)\n", msg);
	}
};