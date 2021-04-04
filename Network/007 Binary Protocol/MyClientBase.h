#pragma once

#include "MySocket.h"
#include "MyPacket.h"

class MyClientBase : public MyNonCopyable {
public:

	void connect(const char* hostname, uint16_t port);

	virtual void onConnected() {}
	virtual void onRecvPacket(MyPacketType packetType, const std::vector<char>& buf) {}

	void acceptFromListenSock(MySocket& listenSock);

	void sendPacket(MyPacket& pkt);

	enum class Status {
		None,
		Connecting,
		Connected,
		Closed,
	};

	void onUpdate(MyPollFD& fd);
	void getPollFD(MyPollFD& pf);

	void close();
	bool isValid();

private:
	void _onConnected();
	void _onRecv();
	void _onSend();

	Status _status = Status::None;
	
	MyPollFD _pollfd;

	std::vector<char> _tmp;
	std::vector<char> _recvBuf;
	std::vector<char> _sendBuf;
	size_t _sendBufOffset = 0;
	MySocket _sock;
};