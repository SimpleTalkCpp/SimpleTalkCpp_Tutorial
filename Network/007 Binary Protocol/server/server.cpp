#include "../MyClientBase.h"

bool g_quit = false;

class MyServer : public MyNonCopyable {
public:
	class Client : public MyClientBase {
	public:
		virtual void onRecvPacket(MyPacketType packetType, const std::vector<char>& buf) override {
			switch (packetType) {
				case MyPacketType::Hello: {
					MyPacket_Hello pkt;
					pkt.fromBuffer(buf);
				}break;

				case MyPacketType::Chat:{
					MyPacket_Chat pkt;
					pkt.fromBuffer(buf);
					printf("chat: \"%s\"\n", pkt.msg.c_str());
					for (auto& t : pkt.toUsers) {
						printf("  to: \"%s\"\n", t.c_str());
					}

					// toupper and send to all
					for(auto& ch : pkt.msg) {
						ch = toupper(ch);
					}

					_server->sendToAll(pkt);
				}break;
			}
		}

		MyServer* _server = nullptr;
	};


	void sendToAll(MyPacket& pkt) {		
		for(auto& c : _clients) {
			c->sendPacket(pkt);
		}
	}

	void run() {
		_listenSock.createTCP();
		_listenSock.setNonBlocking(true);

		MySocketAddr addr;
		addr.setIPv4(0,0,0,0);
		addr.setPort(3300);
		_listenSock.bind(addr);
		_listenSock.listen();
		printf("listening ...\n");

		while (!g_quit) {
			auto n = _clients.size();
			_pollfds.resize(n + 1);

			{
				for (size_t i = 0; i < _clients.size(); i++) {
					auto& c = _clients[i];
					if (!c->isValid()) {
						if (_clients.size() > 1)
							std::swap(c, _clients.back());
						_clients.resize(_clients.size() - 1);
						i--;
						continue;
					}
					c->getPollFD(_pollfds[i]);
				}
			}

			_pollfds[n].reset(_listenSock, true, false);

			int ret = my_poll(_pollfds.data(), _pollfds.size(), 1000);

			{
				int i = 0;
				for (auto& c : _clients) {
					try {
						c->onUpdate(_pollfds[i]);
					} catch(...) {
						c->close();
					}
					i++;
				}
			}

			if (_pollfds[n].canRead()) {
				_clients.emplace_back(new Client);
				auto& newClient = _clients.back();
				newClient->_server = this;
				newClient->acceptFromListenSock(_listenSock);
				printf("accepted\n");
			}
		}		
	}

	MySocket _listenSock;
	std::vector< std::unique_ptr<Client> > _clients;
	std::vector< MyPollFD > _pollfds;
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

void test_serializer();

int main(int argv, const char* argc[]) {
	test_serializer();

	signal(SIGTERM, my_singal_handler); // kill process
	signal(SIGINT,  my_singal_handler); // Ctrl + C

	MyServer server;
	server.run();
	return 0;
}

