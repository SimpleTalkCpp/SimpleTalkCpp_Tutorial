#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <algorithm>
#include <vector>
#include <mutex>
#include <condition_variable>

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

inline
bool primeNumber(int64_t v) {
	for (int64_t i = 2; i < v; i++) {
		if (v % i == 0)
			return false;
	}
	return true;
}

const int64_t kPrimeStart = 1000000000LL;
const int64_t kThreadCount = 4;
const int64_t kBatchSize = 10;
