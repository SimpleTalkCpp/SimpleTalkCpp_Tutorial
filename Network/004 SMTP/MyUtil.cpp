#include "MyUtil.h"

const char* MyUtil::getStringToken(std::string& token, const char* input, char sep) {
	token.clear();

	auto* p = input;

	//trim
	for (;*p; p++) {
		if (*p != ' ')
			break;
	}

	auto* start = p;
		
	// find sep
	for(; *p; p++) {
		if (sep == *p) {
			break;
		}
	}

	token.assign(start, p);

	printf("token = [%s]\n", token.c_str());
	return p;
}

const char* MyUtil::getUpperStringToken(std::string& token, const char* input, char sep)
{
	auto ret = getStringToken(token, input, sep);
	toUpper(token);
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

void MyUtil::toUpper(std::string& s) {
	for (auto& c : s) {
		c = toupper(c);
	}
}
