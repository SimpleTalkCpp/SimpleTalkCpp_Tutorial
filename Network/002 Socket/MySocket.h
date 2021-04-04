#pragma once

#include "my_common.h"

class MySocketAddr {
public:
	MySocketAddr();

	void setPort(uint16_t port);
	uint16_t port() const;

	void setIPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

	struct sockaddr _addr;
};

class MySocket : public MyNonCopyable {
public:
	
#ifdef _WIN32
	typedef int socklen_t;
#else
	typedef int SOCKET;
	const SOCKET INVALID_SOCKET = -1;
#endif

	~MySocket() { close(); }

	void createUDP();
	void createTCP();
	void close();

	void bind(const MySocketAddr& addr);

	void send(const MySocketAddr& addr, const char* data, size_t dataSize);
	void recv(std::vector<char> & buf, size_t bytesToRecv);

private:
	SOCKET _sock = INVALID_SOCKET;
};
