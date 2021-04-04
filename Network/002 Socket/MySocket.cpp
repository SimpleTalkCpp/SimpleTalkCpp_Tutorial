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

	printf("bind %u\n", addr.port());
}

void MySocket::send(const MySocketAddr& addr, const char* data, size_t dataSize) {
	if (dataSize > INT_MAX)
		throw MyError("send dataSize is too big");

	int ret =::sendto(_sock, data, (int)dataSize, 0, &addr._addr, sizeof(addr._addr));
	if (ret < 0) {
		throw MyError("send");
	}
}

void MySocket::recv(std::vector<char> & buf, size_t bytesToRecv) {
	buf.clear();
	if (bytesToRecv > INT_MAX)
		throw MyError("recv bytesToRecv is too big");
	buf.resize(bytesToRecv);

	MySocketAddr addr;
	socklen_t addrLen = sizeof(addr._addr);	
	int ret = ::recvfrom(_sock, buf.data(), (int)bytesToRecv, 0, &addr._addr, &addrLen);
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
