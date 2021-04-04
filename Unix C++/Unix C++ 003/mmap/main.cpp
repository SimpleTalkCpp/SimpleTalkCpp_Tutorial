#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "my_common.h"

int main() {
	int fd = open("test.txt", O_EXCL);
	if (fd < 0) {
		throw MyError("cannot open file");
	}

	struct stat s;
	if (fstat(fd, &s) < 0) {
		throw MyError("fstat error");
	}

	std::cout << "file size = " << s.st_size << std::endl;

	auto* m = (char*)mmap(0, s.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (!m) {
		throw MyError("mmap error");
	}

	printf("file content = ");
	for (int i = 0; i < s.st_size; i++) {
		printf("%c", m[i]);
	}
	printf("\n");

	munmap(m, s.st_size);
	close(fd);
	return 0;
}