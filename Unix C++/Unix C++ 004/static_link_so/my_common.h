#include <exception>
#include <iostream>
#include <stdio.h>

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