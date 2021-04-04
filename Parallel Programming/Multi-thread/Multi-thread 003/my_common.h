#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>

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

	void print() {
		printf("time: %f\n", get());
	}

private:
	uint64_t getTick() { 
		return GetTickCount64();
	}

	uint64_t m_start;
};

inline DWORD myGetThreadId() {
	return GetThreadId(GetCurrentThread());
}
