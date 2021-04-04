#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#ifdef _WIN32

	#include <Windows.h>
	#include <Windowsx.h>
	#include <conio.h>

	#undef max
	#undef min
#endif

#include <stdint.h>
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <signal.h>
#include <codecvt>
#include <cassert>
#include <limits>
#include <algorithm>

// C++11 Multi-thread library
#include <thread>
#include <mutex>
#include <condition_variable>

//------------------------
class MyNonCopyable {
public:
	MyNonCopyable() = default;
private:
	MyNonCopyable(const MyNonCopyable&) = delete;
	void operator=(const MyNonCopyable&) = delete;
};

class MyError : public std::exception {
public:
	MyError(const char* msg = "") {
		printf("MyError(%s)\n", msg);
	}
};

class MyTimer {
public:
	MyTimer() { 
		reset(); 
	}

	void reset() {
		m_start = getTick();
	}

	double get() {
		auto c = getTick();
		return (c - m_start) / 1000.0f;
	}

private:

	uint64_t getTick() { 
		return GetTickCount64();
	}

	uint64_t m_start;
};

