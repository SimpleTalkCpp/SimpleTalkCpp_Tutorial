#pragma once

#include <stdint.h>
#include <string>
#include <exception>

class MyError : public std::exception {
public:
	MyError(const char* msg = "") {
		printf("MyError(%s)\n", msg);
	}
};