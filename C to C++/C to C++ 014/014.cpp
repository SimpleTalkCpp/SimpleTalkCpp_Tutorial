#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>
#include <stdlib.h>
#include <memory>

#include <iostream>
#include <iomanip>

#include <string>

#include <fstream>

class MyOutStream {
public:

};

inline
MyOutStream& operator << (MyOutStream& s, int v) {
	printf("%d", v);
	return s;
}

inline
MyOutStream& operator << (MyOutStream& s, float v) {
	printf("%f", v);
	return s;
}

void input_example() {
	std::cout << "Please input a number: ";
	int i;
	std::cin >> i;
	std::cout << "i = " << i << "\n";
}

void setw_example() {
	int i = 100;
	std::cout << "[" << i << "]\n";
	std::cout << "[" << std::setw(10) << i << "]" << i << "\n";
}

class Point {
public:
	int x = 0;
	int y = 0;
};

std::ostream& operator << (std::ostream& s, const Point & pt) {
	s << "Point(" << pt.x << ", " << pt.y << ")";
	return s;
}

void cout_custom_class() {
	Point pt;
	pt.x = 1;
	pt.y = 2;
	std::cout << pt << "\n";

	std::wcout << L"wchar example\n";
}

void string_example() {
	std::string a("Hello");
	std::string b = a;

	//std::wstring wchar_t string 

	b.append(" World");
	b += " 123";

	std::cout << b << "\n";

	const char* sz = b.c_str();
	printf("c_str = %s\n", sz);
}

int main() {
	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}