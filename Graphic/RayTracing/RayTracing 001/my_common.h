#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#ifdef _WIN32

	#include <Windows.h>
	#include <Windowsx.h>
	#include <conio.h>

	#include <GL/gl.h>
	#include <GL/glu.h>
	#pragma comment(lib, "Opengl32.lib")
	#pragma comment(lib, "Glu32.lib")

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

const float my_PI = 3.141593f;

inline void my_sleep(int sec) {
#ifdef _WIN32
	Sleep(sec * 1000);
#else
	sleep(sec);
#endif
}

inline
float my_rad(float deg) {
	return deg / 360.0f * 2 * 3.14159f;
}

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

inline
uint64_t my_to_uint64(const LARGE_INTEGER& v) {
	return static_cast<uint64_t>(v.HighPart) << 32 | v.LowPart;
}

class MyHiResTimer {
public:
	MyHiResTimer() { 
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

inline
void my_loadfile(std::string& outStr, const char* filename) {
	auto* f = fopen(filename, "rb");
	if (!f)
		throw MyError("cannot open file");

	fseek(f, 0, SEEK_END);
	auto filesize = ftell(f);

	fseek(f, 0, SEEK_SET);

	outStr.resize(filesize);
	if (fread(&*outStr.begin(), filesize, 1, f) < 0) {
		fclose(f);
		throw MyError("error read file");
	}
	fclose(f);
}

inline
const char* my_getline(std::string& outLine, const char* sz) {
	outLine.clear();
	if (!sz)
		return nullptr;

	auto* e = strchr(sz, '\n');
	if (!e) {
		outLine = sz;
		return nullptr;
	} else {
		outLine.assign(sz, e);
		return e + 1;
	}
}

inline
const char* my_gettoken(std::string& outToken, const char* sz, char delimiter = ' ') {
	outToken.clear();
	if (!sz)
		return nullptr;

	//trim spaces
	for (;*sz;sz++) {
		if (*sz != ' ')
			break;;
	}

	auto* e = strchr(sz, delimiter);
	if (!e) {
		outToken = sz;
		return nullptr;
	} else {
		outToken.assign(sz, e);
		return e + 1;
	}
}

inline 
bool my_math_eq(float a, float b, float epsilon = FLT_EPSILON) {
	return std::abs(a-b) < epsilon;
}