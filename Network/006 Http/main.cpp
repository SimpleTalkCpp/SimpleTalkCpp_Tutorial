#include "MySocket.h"
#include "MyUtil.h"
#include "MyServer.h"

void my_singal_handler(int sig) {
	printf("my_singal_handler %d\n", sig);
	switch (sig) {
		case SIGINT:
		case SIGTERM: {
			MyServer::s_quit = true;
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