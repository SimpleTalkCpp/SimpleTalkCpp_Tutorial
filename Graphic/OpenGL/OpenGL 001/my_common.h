#pragma once

#ifdef _WIN32
	#include <Windows.h>
	#include <Windowsx.h>
	#include <conio.h>

	#include <GL/gl.h>
	#include <GL/glu.h>
	#pragma comment(lib, "Opengl32.lib")

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

inline void my_sleep(int sec) {
#ifdef _WIN32
	Sleep(sec * 1000);
#else
	sleep(sec);
#endif
}

inline
uint64_t my_to_uint64(const LARGE_INTEGER& v) {
	return ((uint64_t)v.HighPart) << 32 | v.LowPart;
}

class MyTimer {
public:
	MyTimer() { 
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		m_freq = my_to_uint64(f);
		reset(); 
	}

	void reset() {
		m_start = getTick();
	}

	double get() {
		auto c = getTick();
		return (double)(c - m_start) / m_freq;
	}

private:

	uint64_t getTick() { 
		LARGE_INTEGER v;
		QueryPerformanceCounter(&v);
		return my_to_uint64(v);
	}

	uint64_t m_freq;
	uint64_t m_start;
};

template<typename T>
inline typename std::underlying_type<T>::type& my_enum_to_int(T& v) {
	return *reinterpret_cast<typename std::underlying_type<T>::type*>(&v);
}

template<typename T>
inline const typename std::underlying_type<T>::type& my_enum_to_int(const T& v) {
	return *reinterpret_cast<const typename std::underlying_type<T>::type*>(&v);
}
