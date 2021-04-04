#pragma once

#include "my_common.h"

struct MyUtil {

static void toUpper(std::string& s);

static const char* getStringToken(std::string& token, const char* input, char sep);
static const char* getUpperStringToken(std::string& token, const char* input, char sep);

static const char* getLine(std::string& outLine, const char* input);

};