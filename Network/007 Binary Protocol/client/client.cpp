#include "../MyClientBase.h"

bool g_quit = false;

class MyClient : public MyClientBase {
public:
	void update() {
		MyPollFD pf;
		getPollFD(pf);

		if (my_poll(&pf, 1, 100))
			onUpdate(pf);
	}

	virtual void onConnected() override {
		MyPacket_Hello pkt;
		pkt.version = 100;
		sendPacket(pkt);
		_isConnected = true;
	}

	bool isConnected() const {
		return _isConnected;
	}

	virtual void onRecvPacket(MyPacketType packetType, const std::vector<char>& buf) override {
		switch (packetType) {
			case MyPacketType::Chat:{
				MyPacket_Chat pkt;
				pkt.fromBuffer(buf);
				printf("chat: \"%s\"\n", pkt.msg.c_str());
				for (auto& t : pkt.toUsers) {
					printf("  to: \"%s\"\n", t.c_str());
				}
			}break;
		}
	}

private:
	bool _isConnected = false;
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
	
	if (argv < 2) {
		printf("client.exe <hostname>");
		return -1;
	}

	MyClient client;
	client.connect(argc[1], 3300);

	auto currentTime = time(nullptr);
	auto lastTime = currentTime;

	while (!g_quit) {
		client.update();

		currentTime = time(nullptr);
		if (currentTime - lastTime > 5) {
			lastTime = currentTime;

			if (client.isConnected()) {
				printf("time's up\n");

				char sz[256];
				snprintf(sz, 256, "My Message %d", (int)currentTime);

				MyPacket_Chat pkt;
				pkt.msg = sz;
				pkt.toUsers.emplace_back("Tom");
				pkt.toUsers.emplace_back("John");
				client.sendPacket(pkt);
			}
		}
	}
}

