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
	static const SOCKET kInvalidSocket = INVALID_SOCKET;
#else
	typedef int SOCKET;
	static const SOCKET kInvalidSocket = -1;
#endif

	~MySocket() { close(); }

	bool isValid() { return _sock != kInvalidSocket; }

	void createUDP();
	void createTCP();
	void close();

	void bind(const MySocketAddr& addr);
	void listen(int backlog = 64);

	bool connect(const MySocketAddr& addr);
	bool connect(const char* hostname, uint16_t port);

	bool accept(MySocket & acceptedSocket);

	void sendto(const MySocketAddr& addr, const char* data, size_t dataSize);
	void sendto_c_str(const MySocketAddr& addr, const char* data) { sendto(addr, data, strlen(data)); }

	size_t send(const char* data, size_t dataSize);
	size_t send(const std::vector<char> & data)	{ return send(data.data(), data.size()); }
	size_t send_c_str(const char* data)			{ return send(data, strlen(data)); }

	template<size_t N>
	size_t send(const char (&sz)[N])				{ return N ? send(sz, N-1) : 0; }

	size_t availableBytesToRead();

	void recv(char* buf, size_t bytesToRecv);
	void recv(std::vector<char> & buf, size_t bytesToRecv);

	void appendRecv(std::vector<char> & buf, size_t bytesToRecv);

	void recvfrom(MySocketAddr& addr, std::vector<char> & buf, size_t bytesToRecv);

	void setNonBlocking(bool b);

	SOCKET sock() { return _sock; }

private:
	SOCKET _sock = kInvalidSocket;
};


#ifdef _WIN32
	typedef WSAPOLLFD MyPollFD_Base;
#else
	typedef struct pollfd MyPollFD_Base;
#endif

struct MyPollFD : MyPollFD_Base {
	using Base = MyPollFD_Base;

	MyPollFD() {
		fd = MySocket::kInvalidSocket;
		events = 0;
		revents = 0;
	}

	void reset(MySocket& sock, bool read, bool write) {
		fd = sock.sock();
		revents = 0;
		events = 0;
		if (read ) events |= POLLIN;
		if (write) events |= POLLOUT;
	}

	bool canRead () { return revents & POLLIN;  }
	bool canWrite() { return revents & POLLOUT; }
};

static_assert(sizeof(MyPollFD) == sizeof(MyPollFD_Base), "cannot add member in MyPollFD");

#ifdef _WIN32
inline int my_poll(MyPollFD* pollfd, size_t pollfdCount, int timeoutMillisecond) {
	int ret = WSAPoll(pollfd, pollfdCount, timeoutMillisecond);
	if (ret < 0) {
		throw MyError("poll");
	}
	return ret;
}

#else

inline int my_poll(MyPollFD* pollfd, size_t pollfdCount, int timeoutMillisecond) {
	int ret = ::poll(pollfd, pollfdCount, timeoutMillisecond);
	if (ret < 0)
		throw MyError("poll");
	return ret;
}
#endif