#include "MySocket.h"

bool isServer = false;

void testUDP() {
	MySocket sock;
	sock.createUDP();

	if (isServer) {
		MySocketAddr addr;
		addr.setIPv4(0,0,0,0);
		addr.setPort(3000);

		sock.bind(addr);

		std::vector<char> buf;

		for(;;) {
			size_t n = sock.availableBytesToRead();
			if (n == 0) {
				//printf("wait..\n");
				my_sleep(1);
				continue;
			}

			sock.recv(buf, n);
			buf.push_back(0);
			printf("recv %d: %s\n", (int)n, buf.data());
		}

	}else{
		MySocketAddr addr;
		addr.setIPv4(127,0,0,1);
		addr.setPort(3000);

		sock.sendto_c_str(addr, "1234ABCD");
	}
}

void testTCP() {
	if (isServer) {
		MySocket listenSock;
		listenSock.createTCP();

		MySocketAddr addr;
		addr.setIPv4(0,0,0,0);
		addr.setPort(3000);

		listenSock.bind(addr);
		listenSock.listen();

		MySocket client;
		listenSock.accept(client);

		std::vector<char> buf;

		for(;;) {
			size_t n = client.availableBytesToRead();
			if (n == 0) {
				//printf("wait..\n");
				my_sleep(1);
				continue;
			}

			client.recv(buf, n);
			buf.push_back(0);
			printf("recv %d: %s\n", (int)n, buf.data());

			for (auto& c : buf) {
				c = toupper(c);
			}

			client.send(buf);
		}

	}else{
		MySocket sock;
		sock.createTCP();

		//MySocketAddr addr;
		//addr.setIPv4(127,0,0,1);
		//addr.setPort(3000);
		//sock.connect(addr);

		sock.connectIPv4("www.gov.hk", 80);
		sock.send_c_str("GET /index.html HTTP/1.1\r\n"
						"Host: www.gov.hk\r\n"
						"\r\n");

		std::vector<char> buf;

		for(;;) {
			size_t n = sock.availableBytesToRead();
			if (n == 0) {
				//printf("wait..\n");
				my_sleep(1);
				continue;
			}

			sock.recv(buf, n);
			buf.push_back(0);
			printf("recv %d: %s\n", (int)n, buf.data());
		}
	}
}

void testDomainName() {
	std::vector<MySocketAddr> addrs;
	myGetHostByNameIPv4(addrs, "www.gov.hk");

	for (auto& a : addrs) {
		a.print();
	}
}

int main(int argv, const char* argc[]) {
	if (argv > 1 && 0 == strcmp(argc[1], "-server")) {
		isServer = true;
	}

	//testUDP();
	testTCP();

	//testDomainName();

	printf("\n======= Program Ended ======\n");

#ifdef _WIN32
	if (IsDebuggerPresent()) {
		printf("Press any key to Exit");
		_getch();
	}
#endif

	return 0;
}