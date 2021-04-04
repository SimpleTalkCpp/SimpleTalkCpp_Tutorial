#include "MySocket.h"

class MySocketInit {
public:
#ifdef _WIN32
	MySocketInit() {
		WSADATA data;
		if (0 != WSAStartup(MAKEWORD(2,2), &data)) {
			throw MyError("WSAStartup");
		}
	}

	~MySocketInit() {
		WSACleanup();
	}

	static void init() {
		static MySocketInit s;
	}
#else
	static void init() {
	}
#endif
};

void myGetHostByNameIPv4(std::vector<MySocketAddr> & outAddrs, const char* hostname, const char* service /*= nullptr*/) {
	outAddrs.clear();

	MySocketInit::init();

	// The gethostbyname function has been deprecated 
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	struct addrinfo* result = nullptr;

	if (0 != getaddrinfo(hostname, service, &hints, &result)) {
		throw MyError("getaddrinfo");
	}

	if (!result)
		return;

	for (auto* r = result; r; r = r->ai_next) {
		outAddrs.emplace_back();
		outAddrs.back()._addr = *r->ai_addr;
	}

	freeaddrinfo(result);
}

void MySocket::createUDP() {
	close();

	MySocketInit::init();
	_sock = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (_sock == INVALID_SOCKET) {
		throw MyError("createUDP");
	}
}

void MySocket::createTCP() {
	close();

	MySocketInit::init();
	_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == INVALID_SOCKET) {
		throw MyError("createTCP");
	}
}

void MySocket::bind(const MySocketAddr& addr) {
	int ret = ::bind(_sock, &addr._addr, sizeof(addr._addr));
	if (ret != 0) {
		throw MyError("bind");
	}

//	printf("bind %u\n", addr.port());
}

void MySocket::listen(int backlog) {
	int ret = ::listen(_sock, backlog);
	if (ret != 0) {
		throw MyError("listen");
	}
	printf("socket listening ...\n");
}

void MySocket::connect(const MySocketAddr& addr) {
	int ret = ::connect(_sock, &addr._addr, sizeof(addr._addr));
	if (ret < 0) {
		throw MyError("connect");		
	}
//	printf("connect\n");
}

void MySocket::connect(const char* hostname, uint16_t port) {
	MySocketAddr addr;
	addr.setIPv4(hostname, nullptr);
	addr.setPort(port);

	connect(addr);
}

bool MySocket::accept(MySocket & acceptedSocket) {
	acceptedSocket.close();

	auto c = ::accept(_sock, nullptr, nullptr);
	if (c == INVALID_SOCKET)
		return false;

//	printf("accept\n");
	acceptedSocket._sock = c;
	return true;
}

void MySocket::sendto(const MySocketAddr& addr, const char* data, size_t dataSize) {
	if (dataSize > INT_MAX)
		throw MyError("send dataSize is too big");

	int ret =::sendto(_sock, data, (int)dataSize, 0, &addr._addr, sizeof(addr._addr));
	if (ret < 0) {
		throw MyError("send");
	}
}

size_t MySocket::send(const char* data, size_t dataSize) {
	if (dataSize > INT_MAX)
		throw MyError("send dataSize is too big");

	int ret =::send(_sock, data, (int)dataSize, 0);
	if (ret < 0) {
		throw MyError("send");
	}
	return ret;
}

size_t MySocket::availableBytesToRead() {
#ifdef _WIN32
	u_long n = 0;
	if (0 != ::ioctlsocket(_sock, FIONREAD, &n))
		throw MyError("availableBytesToRead");
	return static_cast<size_t>(n);
#else
	int n = 0;
	if (0 != ::ioctl(_sock, FIONREAD, &n))
		throw MyError("availableBytesToRead");
	return static_cast<size_t>(n);
#endif
}

void MySocket::recvfrom(MySocketAddr& addr, std::vector<char> & buf, size_t bytesToRecv) {
	buf.clear();
	if (bytesToRecv > INT_MAX)
		throw MyError("recv bytesToRecv is too big");

	buf.resize(bytesToRecv);
	socklen_t addrLen = sizeof(addr._addr);	
	int ret = ::recvfrom(_sock, buf.data(), (int)bytesToRecv, 0, &addr._addr, &addrLen);
	if (ret < 0) {
		throw MyError("recv");
	}
}

void MySocket::setNonBlocking(bool b)
{
#ifdef _WIN32
	u_long v = b ? 1 : 0;
	if (0 != ::ioctlsocket(_sock, FIONBIO, &v))
		throw MyError("setNonBlocking");
#else
	long v = b ? 1 : 0;
	if (0 != ::ioctl(_sock, FIONBIO, &v))
		throw MyError("setNonBlocking");
#endif		
}

void MySocket::recv(std::vector<char> & buf, size_t bytesToRecv) {
	buf.clear();
	if (bytesToRecv > INT_MAX)
		throw MyError("recv bytesToRecv is too big");

	buf.resize(bytesToRecv);
	recv(buf.data(), bytesToRecv);
}

void MySocket::recv(char* buf, size_t bytesToRecv) {
	if (bytesToRecv > INT_MAX)
		throw MyError("recv bytesToRecv is too big");
	int ret = ::recv(_sock, buf, (int)bytesToRecv, 0);
	if (ret < 0) {
		throw MyError("recv");
	}
}

void MySocket::close() {
	if (_sock != INVALID_SOCKET) {
		#ifdef _WIN32
			::closesocket(_sock);
		#else
			::close(_sock);
		#endif
		_sock = INVALID_SOCKET;
	}
}

MySocketAddr::MySocketAddr() {
	memset(&_addr, 0, sizeof(_addr));
}

void MySocketAddr::setPort(uint16_t port) {
	*reinterpret_cast<uint16_t*>(_addr.sa_data) = htons(port);
}

uint16_t MySocketAddr::port() const {
	return ntohs(*reinterpret_cast<const uint16_t*>(_addr.sa_data));
}

void MySocketAddr::setIPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	_addr.sa_family = AF_INET;
	_addr.sa_data[2] = a;
	_addr.sa_data[3] = b;
	_addr.sa_data[4] = c;
	_addr.sa_data[5] = d;
}

void MySocketAddr::setIPv4(const char* hostname, const char* service) {
	std::vector<MySocketAddr> addrs;
	myGetHostByNameIPv4(addrs, hostname, service);
	if (!addrs.size())
		throw MyError("setIPv4");

	*this = addrs[0];
}

void MySocketAddr::print() const {
	if (_addr.sa_family == AF_INET) {
		auto* p = reinterpret_cast<const uint8_t*>(_addr.sa_data);
		printf("IPv4 %u.%u.%u.%u : %u\n", p[2], p[3], p[4], p[5], port());
	}
}
