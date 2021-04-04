#include "MyUtil.h"

void MyUtil::utfConvert(std::wstring & o, const char* src, size_t srcLen)
{
	auto* p = src;
	auto* e = p + srcLen;
	while (p < e) {
		auto c = (uint8_t)*p;
		if (c < 0xC0) {
			o.push_back(c & 0x7F);
			p++;
		}
		else if (c < 0xE0) {
			if (p + 2 > e) throw MyError("decodeUtf8");
			auto a = (wchar_t)*p; p++;
			auto b = (wchar_t)*p; p++;
			wchar_t v = ((a & 0x1F) << 6) | (b & 0x3F);
			o.push_back(v);
		}
		else if (c < 0xF0) {
			if (p + 3 > e) throw MyError("decodeUtf8");
			auto a = (wchar_t)*p; p++;
			auto b = (wchar_t)*p; p++;
			auto c = (wchar_t)*p; p++;
			wchar_t v = ((a & 0x0F) << 12) | ((b & 0x3F) << 6) | (c & 0x3F);
			o.push_back(v);
		}
		else {
			if (p + 4 > e) throw MyError("decodeUtf8");
			auto a = (wchar_t)*p; p++;
			auto b = (wchar_t)*p; p++;
			auto c = (wchar_t)*p; p++;
			auto d = (wchar_t)*p; p++;
			wchar_t v = ((a & 0x07) << 18) | ((b & 0x3F) << 12) | ((c & 0x3F) << 6) | (c & 0x3F);
			o.push_back(v);
		}
	}
}

void MyUtil::utfConvert(std::string & o, const wchar_t* src, size_t srcLen)
{
	auto* p = src;
	auto* e = p + srcLen;
	for (; p < e; p++) {
		auto c = (uint32_t)*p;
		if (c < 0x80) {
			// 1 byte
			o.push_back(static_cast<char>(c));
		}
		else if (c < 0x800) {
			// 2 bytes
			char t[] = {
				static_cast<char>((c >> 6) & 0x1F | 0xC0),
				static_cast<char>(c & 0x3F | 0x80)
			};
			o.append(t, t + 2);
		}
		else if (c < 0x10000) {
			char t[] = {
				static_cast<char>((c >> 12) & 0x0F | 0xE0),
				static_cast<char>((c >> 6) & 0x3F | 0x80),
				static_cast<char>(c & 0x3F | 0x80)
			};
			o.append(t, t + 3);
		}
		else {
			char t[] = {
				static_cast<char>((c >> 18) & 0x07 | 0xF0),
				static_cast<char>((c >> 12) & 0x3F | 0x80),
				static_cast<char>((c >> 6) & 0x3F | 0x80),
				static_cast<char>(c & 0x3F | 0x80)
			};
			o.append(t, t + 4);
		}
	}
}

const char* MyUtil::getStringToken(std::string& token, const char* input, char separator) {
	token.clear();

	auto* p = input;

	//trim spaces
	for (;*p; p++) {
		if (*p != ' ' && *p != '\t')
			break;
	}

	auto* start = p;
		
	// find separator
	for(; *p; p++) {
		if (separator == *p) {
			break;
		}
	}

	token.assign(start, p);

	//printf("token = [%s]\n", token.c_str());
	return p;
}

const char* MyUtil::getUpperStringToken(std::string& token, const char* input, char separator) {
	auto ret = getStringToken(token, input, separator);
	toUpperString(token);
	return ret;
}

const char* MyUtil::getLine(std::string& outLine, const char* input) {
	outLine.clear();
	auto* e = strstr(input, "\r\n");
	if (!e) {
		outLine = input;
		return input + strlen(input);
	}
	outLine.assign(input, e);
	return e + 2;
}

bool MyUtil::stringStartsWith(const char* s, const char* prefix)
{
	int n = strlen(prefix);
	return 0 == strncmp(s, prefix, n);
}

void MyUtil::stringAppendNumber(std::string& o, int v)
{
	char tmp[64 + 1];
	snprintf(tmp, 64, "%d", v);
	tmp[64] = 0;
	o.append(tmp);
}

void MyUtil::toUpperString(std::string& s) {
	for (auto& c : s) {
		c = toupper(c);
	}
}

void MyUtil::getCurrentDir(std::string& outPath) {
#ifdef _WIN32
	wchar_t tmp[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, tmp);
	tmp[MAX_PATH] = 0;
	utfConvert(outPath, tmp);
	toUnixPath(outPath);
#else
	char tmp[PATH_MAX+1];
	if (!getcwd(tmp, PATH_MAX))
		throw MyError("getcwd");
	tmp[PATH_MAX] = 0;
	outPath = tmp;
#endif	
}

void MyUtil::getAbsPath(std::string& outPath, const char* inPath) {
#ifdef _WIN32
	std::wstring inPathW;
	utfConvert(inPathW, inPath);

	wchar_t tmp[MAX_PATH + 1];
	GetFullPathName(inPathW.c_str(), MAX_PATH, tmp, nullptr);
	tmp[MAX_PATH] = 0;
	utfConvert(outPath, tmp);
	toUnixPath(outPath);
#else
	char tmp[PATH_MAX+1];
	if (!realpath(inPath, tmp))
		throw MyError("realpath");
	tmp[PATH_MAX] = 0;
	outPath = tmp;
#endif
}

void MyUtil::toUnixPath(std::string& s)
{
	for (auto& c : s) {
		if (c == '\\')
			c = '/';
	}
}

int MyUtil::valueOfHex(char v) {
	if (v >= '0' && v <= '9')
		return v - '0';
	if (v >= 'A' && v <= 'F')
		return v - 'A' + 10;
	if (v >= 'a' && v <= 'f')
		return v - 'a' + 10;
	return 0; // error ?
}

void MyUtil::decodeURI(std::string& outStr, const char* sz) {
	outStr.clear();
	if (!sz)
		return;

	auto* p = sz;
	for (;*p; p++) {
		if (*p == '%') {
			if (!p[1] || !p[2]) {
				throw MyError("invalid URI");
			}

			char v = (valueOfHex(p[1]) << 4) | valueOfHex (p[2]);
			outStr.push_back(v);
			p+=2;
		}else{
			outStr.push_back(*p);
		}
	}
}

