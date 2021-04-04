#include "Utf.h"
#include <conio.h>
#include <iostream>
#include <clocale>

#include <Windows.h>
#include <codecvt>

void dump(const char* s) {
	for (; *s; s++) {
		printf("[%X, %c]\n", (uint8_t)*s, *s);
	}
}

void dump(const char32_t* s) {
	for (; *s; s++) {
		printf("[%04X]\n", (uint32_t)*s);
	}
}

int main() {
	SetConsoleOutputCP(CP_UTF8);

	const char*    sz8 = u8"A你";
	const wchar_t* szW = L"A你";
	const char32_t* sz32 = U"A你";
	
	dump(sz32);

	std::u32string s32;
	Utf::decodeUtf8(s32, sz8);
	dump(s32.c_str());
	
 	std::string s8;
	Utf::encodeUtf8(s8, sz32);
	dump(s8.c_str());

	std::cout << sz8 << "\n";
	std::cout << s8 << "\n";

	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convertor;
		auto s = convertor.to_bytes(szW);
		std::cout << s << "\n";
	}

	//mbstowcs - multi-byte string to wide char string

	{
		char buf[256];
		int n = WideCharToMultiByte(CP_UTF8, 0, szW, 2, buf, 256, nullptr, nullptr);
		buf[n] = 0;
		std::cout << buf << "\n";
	}
	
	printf("\n======== Program End =======\n");
	_getch();
}