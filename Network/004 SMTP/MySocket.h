#pragma once

#include "my_common.h"

class MySocketAddr {
public:
	MySocketAddr();

	void setPort(uint16_t port);
	uint16_t port() const;

	void setIPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

	void setIPv4(const char* hostname, const char* service = nullptr);

	void print() const;

	struct sockaddr _addr;
};

void myGetHostByNameIPv4(std::vector<MySocketAddr> & outAddrs, const char* hostname, const char* service = nullptr);

class MySocket : public MyNonCopyable {
public:
	
#ifdef _WIN32
	typedef int socklen_t;	
#else
	typedef int SOCKET;
	const SOCKET INVALID_SOCKET = -1;
#endif

	~MySocket() { close(); }

	bool isValid() { return _sock != INVALID_SOCKET; }

	void createUDP();
	void createTCP();
	void close();

	void bind(const MySocketAddr& addr);
	void listen(int backlog = 64);

	void connect(const MySocketAddr& addr);
	void connect(const char* hostname, uint16_t port);

	bool accept(MySocket & acceptedSocket);

	void sendto(const MySocketAddr& addr, const char* data, size_t dataSize);
	void sendto_c_str(const MySocketAddr& addr, const char* data) { sendto(addr, data, strlen(data)); }

	void send(const char* data, size_t dataSize);
	void send(const std::vector<char> & data)	{ send(data.data(), data.size()); }
	void send_c_str(const char* data)			{ send(data, strlen(data)); }

	template<size_t N>
	void send(const char (&sz)[N])				{ if(N) send(sz, N-1); }

	size_t availableBytesToRead();

	void recv(char* buf, size_t bytesToRecv);
	void recv(std::vector<char> & buf, size_t bytesToRecv);
	void recvfrom(MySocketAddr& addr, std::vector<char> & buf, size_t bytesToRecv);

	SOCKET sock() { return _sock; }

private:
	SOCKET _sock = INVALID_SOCKET;
};
