#include "my_common.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>

bool quit = false;

void run_loop() {
	auto* f = fopen("test.txt", "wb");
	if (!f) {
		throw MyError("cannot open file to write");
	}

	for (int i = 0; !quit; i++) {
		printf("%d\n", i);
		fprintf(f, "%d\n", i);
		fflush(f);
		sleep(2);
	}

	printf("Quit !\n");
	fprintf(f, "Quit !\n");
	fclose(f);
}

void my_singal_handler(int sig) {
	printf("my_singal_handler %d\n", sig);
	switch (sig) {
		case SIGINT:
		case SIGTERM: {
			syslog(LOG_INFO, "mytestd stopping...");
			quit = true;
		}break;
	}
}

bool my_daemon() {
	umask(0);

	// become seesion leader to lose TTY control
	pid_t pid = fork();
	if (pid < 0) {
		throw MyError("fork error");
	} else if (pid != 0) {
		return false; // parent process
	}
	setsid();

//--------------

	pid = fork();
	if (pid < 0) {
		throw MyError("fork#2 error");
	} else if (pid != 0) {
		return false; // parent process
	}

	{ // redirect stdin, out, err to /dev/null
		int null_fd = open("/dev/null", O_RDWR);
		if (null_fd < 0)
			throw MyError("error open /dev/null");

		dup2(null_fd, 0);
		dup2(null_fd, 1);
		dup2(null_fd, 2);
	}

	return true;
}

int main(int argc, const char* argv[]) {
	openlog("mytestd", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "mytestd start %d");

	signal(SIGTERM, my_singal_handler); // kill process
	signal(SIGINT,  my_singal_handler); // Ctrl + C

	if (argc > 1 && strcmp(argv[1], "-d") == 0) {
		printf("run daemon mode\n");		
		if (!my_daemon())
			return 0;
	} else {
		printf("run console mode\n");
	}
	
	run_loop();
	syslog(LOG_INFO, "mytestd stopped");
	closelog();
	return 0;
}