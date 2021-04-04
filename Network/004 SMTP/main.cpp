#include "MySocket.h"
#include "MyUtil.h"

class MyClient : public MyNonCopyable {
public:
	const int kHandleCommandMaxPerRecv = 4;
	const size_t kRecvOffsetLimit = 4 * 1024;

	enum class State {
		None,
		Ready,
		Mail,
		Data,
	};

	State state = State::None;

	void onConnected() {
		printf("client %p: connected\n", this);
		send("220 MyServer ESMTP MyServer\r\n");
	}

	void onRecv() {
		size_t n = sock.availableBytesToRead();
		if (!n) {
			return;
		}

		if (recvOffset > kRecvOffsetLimit) {
			auto* p = &(*recvBuf.begin());
			size_t dataSize = recvBuf.size() - recvOffset;
			memmove(p, p + recvOffset, dataSize);

			recvBuf.resize(dataSize);
			recvOffset = 0;
		}

		{
			auto oldSize = recvBuf.size();
			recvBuf.resize(recvBuf.size() + n);
			auto& buf = *(recvBuf.begin() + oldSize);
			sock.recv(&buf, n);
		}

		for(int i = 0; i < kHandleCommandMaxPerRecv; i++) {
			if (state == State::Data) {
				onRecvData();
			} else {
				MyUtil::getLine(lineBuf, recvBuf.data() + recvOffset);
				if (!lineBuf.size())
					return;

				recvOffset += lineBuf.size() + 2; // +2 \r\n
				printf("line: [%s]\n", lineBuf.c_str());

				onRecvCommand();
			}
		}
	}

	void onRecvCommand() {
		auto* line = lineBuf.c_str();

		line = MyUtil::getUpperStringToken(token, line, ' ');
		if (!line) {
			sendSyntaxError();
			return;
		}

		if (token == "HELO" || token == "EHLO") {
			state = State::Ready;
			sendOK();
			return;
		}

		if (token == "QUIT") {
			close();
			return;
		}

		if (token == "MAIL") {
			mail.clear();

			line = MyUtil::getUpperStringToken(token, line, ':');
			if (!line || *line != ':') {
				sendSyntaxError();
				return;
			}

			if (token != "FROM") {
				sendSyntaxError();
				return;
			}			
			line++; // remove ':'

			mail.from = line;
			state = State::Mail;
			sendOK();
			return;
		}

		if (token == "RCPT") {
			if (state != State::Mail) {
				sendSyntaxError();
				return;
			}

			line = MyUtil::getUpperStringToken(token, line, ':');
			if (!line || *line != ':' ) {
				sendSyntaxError();
				return;
			}
			line++; // remove ':'

			if (token != "TO") {
				sendSyntaxError();
				return;
			}

			mail.to.emplace_back(line);
			sendOK();
			return;
		}

		if (token == "DATA") {
			if (state != State::Mail) {
				sendSyntaxError();
				return;
			}

			state = State::Data;
			send("354 Start mail input; end with <CRLF>.<CRLF>\r\n");
			return;
		}

		send("502 Command not implemented\r\n");
	}

	void onRecvData() {
		auto* p = recvBuf.data() + recvOffset;

		auto* e = strstr(p, "\r\n.\r\n");
		if (!e) {
			mail.data.append(p);
			recvBuf.clear();
			recvOffset = 0;
			return;
		}

		size_t dataSize = e - p;
		mail.data.append(p, dataSize);
		recvOffset += dataSize;

		printf("mail ready to send, data %s\n", mail.data.c_str());

		state = State::Ready;
		sendOK();
	}

	void sendOK() {
		send("250 OK\r\n");
	}

	void sendSyntaxError() {
		send("500 Syntax error, command unrecognized\r\n");
	}

	void close() {
		printf("client %p: close\n", this);
		sock.close();
	}

	template<size_t N>
	void send(const char (&sz)[N]) {
		printf("send: %s\n", sz);
		sock.send(sz);
	}

	MySocket sock;

	size_t recvOffset = 0;
	std::string recvBuf;
	std::string lineBuf;
	std::string token;

	struct Mail {
		std::string from;
		std::vector<std::string> to;
		std::string data;

		void clear() {
			from.clear();
			to.clear();
			data.clear();
		}
	};

	Mail mail;
};

bool g_quit = false;

class MyServer : public MyNonCopyable {
public:

	const size_t kClientCountMax = 16;

	MyServer() {
		_clients.reserve(kClientCountMax);
	}

	void run() {
		_listenSock.createTCP();

		MySocketAddr addr;
		addr.setIPv4(0,0,0,0);
		addr.setPort(2525);

		_listenSock.bind(addr);
		_listenSock.listen();

		while(!g_quit) {
			fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(_listenSock.sock(), &readfds);
			int n = 1;
			auto max_fd = _listenSock.sock();

			removeClosedClients();

			for (auto& client : _clients) {
				if (!client || !client->sock.isValid())
					continue;

				if (n >= FD_SETSIZE) {
					throw MyError("n >= FD_SETSIZE");
					break;
				}

				auto cs = client->sock.sock();
				if (cs > max_fd)
					max_fd = cs;

				FD_SET(cs, &readfds);
				n++;
			}

			struct timeval tv;
			tv.tv_sec = 3;
			tv.tv_usec = 0;

			// select( nfds <--
			//            Linux - nfds should be set to the highest-numbered file descriptor in any of the three sets, plus 1
			//            Windows - Ignored. The nfds parameter is included only for compatibility with Berkeley sockets
			int ret = ::select(max_fd + 1, &readfds, nullptr, nullptr, &tv);
			if (ret < 0) {
				throw MyError("select");
			}
			if (ret == 0) {
				continue;
			}

			if (FD_ISSET(_listenSock.sock(), &readfds)) {
				if (_clients.size() < kClientCountMax) {
					_clients.emplace_back(new MyClient());
					auto& client = _clients.back();

					try {
						_listenSock.accept(client->sock);
						client->onConnected();
					}catch(...) {
						client->close();
					}
				}
			}

			for (auto& client : _clients) {
				if (!client || !client->sock.isValid())
					continue;

				if (!FD_ISSET(client->sock.sock(), &readfds)) 
					continue;

				try {
					client->onRecv();
				}catch(...) {
					client->close();
				}
			}
		}
	}

	void removeClosedClients() {		
		for (size_t i = 0; i < _clients.size();) {
			auto & c = _clients[i];
			if (c && c->sock.isValid()) {
				i++;
				continue;
			}

			try {
				std::swap(c, _clients.back());
				_clients.resize(_clients.size() - 1);
			} catch(...) {
				printf("exception in removeClosedClients");
			}
		}
	}

private:
	MySocket _listenSock;
	std::vector< std::unique_ptr<MyClient> > _clients;
};

void my_singal_handler(int sig) {
	printf("my_singal_handler %d\n", sig);
	switch (sig) {
		case SIGINT:
		case SIGTERM: {
			g_quit = true;
		}break;
	}
}
int main(int argv, const char* argc[]) {

	signal(SIGTERM, my_singal_handler); // kill process
	signal(SIGINT,  my_singal_handler); // Ctrl + C

	MyServer server;
	server.run();


	printf("\n======= Program Ended ======\n");
#ifdef _WIN32
	if (IsDebuggerPresent()) {
		printf("Press any key to Exit");
		_getch();
	}
#endif

	return 0;
}
