#pragma once

#include "my_common.h"

class MyFileStream {
public:
	MyFileStream() = default;
	~MyFileStream() { close(); }

	bool isOpened() const { return _h != kInvalidHandle; }

	void openRead(const char* filename);
	void close();

	uint64_t fileSize();

	void read(char* outBuf, size_t bytesToRead);
	void read(std::vector<char>& outBuf, size_t bytesToRead);

#ifdef _WIN32
	static const HANDLE kInvalidHandle;
#else
	static const int    kInvalidHandle = -1;
#endif


private:

#ifdef _WIN32
	HANDLE _h = kInvalidHandle;
#else
	int _h = kInvalidHandle;
#endif

};