#include "MyFileStream.h"
#include "MyUtil.h"

#ifdef _WIN32

const HANDLE MyFileStream::kInvalidHandle = INVALID_HANDLE_VALUE;

void MyFileStream::openRead(const char* filename)
{
	close();
	std::wstring filenameW;
	MyUtil::utfConvert(filenameW, filename);

	_h = CreateFile(filenameW.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (_h == kInvalidHandle)
		throw MyError("MyFileStream::openRead");
}

void MyFileStream::close()
{
	if (_h != kInvalidHandle) {
		CloseHandle(_h);
		_h = kInvalidHandle;
	}
}

uint64_t MyFileStream::fileSize()
{
	LARGE_INTEGER t;
	if (!GetFileSizeEx(_h, &t)) {
		return 0;
	}

	return (static_cast<uint64_t>(t.HighPart) << 32) | t.LowPart;
}

void MyFileStream::read(char* outBuf, size_t bytesToRead) {
	if (bytesToRead > std::numeric_limits<DWORD>::max()) {
		throw MyError("bytesToRead is too big");
	}

	DWORD bytesRead = 0;
	if (!ReadFile(_h, outBuf, bytesToRead, &bytesRead, nullptr)) {
		throw MyError("MyFileStream::read");
	}
}

#else

void MyFileStream::openRead(const char* filename) {
	close();
	_h = ::open(filename, 0);
	if (_h == kInvalidHandle)
		throw MyError("MyFileStream::openRead");
}

void MyFileStream::close() {
	if (_h != kInvalidHandle) {
		::close(_h);
		_h = kInvalidHandle;
	}
}

void MyFileStream::read(char* outBuf, size_t bytesToRead) {
	int ret = ::read(_h, outBuf, bytesToRead);
	if (ret < 0) {
		throw MyError("MyFileStream::read");
	}
}

uint64_t MyFileStream::fileSize() {
	struct stat s;
	int ret = fstat(_h, &s);
	if (ret < 0)
		throw MyError("fstat");
	return s.st_size;
}

#endif

//---------------- Common ---------------

void MyFileStream::read(std::vector<char>& outBuf, size_t bytesToRead) {
	outBuf.clear();
	outBuf.resize(bytesToRead);

	try{
		read(outBuf.data(), bytesToRead);
	}catch(...){
		outBuf.clear();
		throw;
	}
}
