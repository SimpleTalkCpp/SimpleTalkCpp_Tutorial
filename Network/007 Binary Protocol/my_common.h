#pragma once

#ifdef _WIN32
	#include <Winsock2.h> // must include before windows.h
	#include <Windows.h>
	#include <Windowsx.h>
	#include <conio.h>
	#include <ws2tcpip.h> // getaddrinfo
	#pragma comment(lib, "Ws2_32.lib")

	#undef max
	#undef min
	
	#include <io.h>
#else
	#include <unistd.h> // sleep()
	#include <arpa/inet.h> // htons
	#include <sys/socket.h>
	#include <netdb.h> // struct addrinfo
	#include <sys/ioctl.h>
	#include <poll.h>
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

template<typename T>
inline typename std::underlying_type<T>::type& my_enum_to_int(T& v) {
	return *reinterpret_cast<typename std::underlying_type<T>::type*>(&v);
}

template<typename T>
inline const typename std::underlying_type<T>::type& my_enum_to_int(const T& v) {
	return *reinterpret_cast<const typename std::underlying_type<T>::type*>(&v);
}
