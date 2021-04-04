#pragma once

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <Commctrl.h>
#include <exception>
#include <memory>
#include <vector>
#include <string>
#include <stdint.h>

template<class T>
inline void my_bzero(T& s) {
	memset(&s, 0, sizeof(s));
}

class MyNonCopyable {
public:
	MyNonCopyable() {}

private:
	MyNonCopyable(const MyNonCopyable&) = delete;
	void operator=(const MyNonCopyable&) = delete;
};

class MyError : public std::exception {
public:
	MyError(const char* msg) {
		printf("%s", msg);
	}

};