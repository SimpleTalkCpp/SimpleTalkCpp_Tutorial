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
			sock.recv(buf, 4);

			buf.push_back(0);
			printf("recv: %s\n", buf.data());
			my_sleep(1);
		}

	}else{
		MySocketAddr addr;
		addr.setIPv4(127,0,0,1);
		addr.setPort(3000);

		sock.send(addr, "1234", 4);
	}
}

int main(int argv, const char* argc[]) {
	if (argv > 1 && 0 == strcmp(argc[1], "-server")) {
		isServer = true;
	}

	testUDP();



	printf("\n======= Program Ended ======\n");

#ifdef _WIN32
	printf("Press any key to Exit");
	_getch();
#endif

	return 0;
}