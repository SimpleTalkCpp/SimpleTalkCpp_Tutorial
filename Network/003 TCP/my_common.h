#pragma once

#ifdef _WIN32
	#include <Winsock2.h> // must include before windows.h
	#include <Windows.h>
	#include <conio.h>
	#include <ws2tcpip.h> // getaddrinfo
	#pragma comment(lib, "Ws2_32.lib")
#else
	#include <unistd.h> // sleep()
	#include <arpa/inet.h> // htons
	#include <sys/socket.h>
	#include <netdb.h> // struct addrinfo
	#include <sys/ioctl.h>
#endif

#include <stdint.h>
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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