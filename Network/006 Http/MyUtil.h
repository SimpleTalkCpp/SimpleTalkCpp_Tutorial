#pragma once

#include "my_common.h"

struct MyUtil {

// string 
static void toUpperString(std::string& s);

static void utfConvert(std::string  & o, const wchar_t* src, size_t srcLen);
static void utfConvert(std::wstring & o, const char*    src, size_t srcLen);

inline static void utfConvert(std::wstring & o, const char*    src) { utfConvert(o, src, strlen(src)); }
inline static void utfConvert(std::string  & o, const wchar_t* src) { utfConvert(o, src, wcslen(src)); }

static const char* getStringToken(std::string& token, const char* input, char separator);
static const char* getUpperStringToken(std::string& token, const char* input, char separator);
static const char* getLine(std::string& outLine, const char* input);

static bool stringStartsWith(const char* s, const char* prefix);

static void stringAppendNumber(std::string& o, int v);

// path
static void getCurrentDir(std::string& outPath);
static void getAbsPath(std::string& outPath, const char* inPath);

static void toUnixPath(std::string& s);

static void decodeURI(std::string& outStr, const char* sz);

static int valueOfHex(char v);

};