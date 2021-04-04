#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <cassert>

#include <exception>

class MyError : public std::exception {
public:
	MyError(int errorCode) {
		m_errorCode = errorCode;
	}

	int m_errorCode;
};

void writeData(FILE* file) {
	if (fprintf(file, "Hello\n") < 0) {
		throw MyError(-1);
	}
}

FILE* try_open_file(const char* filename) {
	FILE* file = fopen(filename, "wb");
	return file;
}

FILE* open_file(const char* filename) {
	FILE* file = try_open_file(filename);
	if (!file) {
		throw MyError(-100);
	}
	return file;
}

void saveFile(const char* filename) {
	FILE* file = fopen(filename, "wb");
	if (!file) {
		throw MyError(-2);
	}

	try {
		writeData(file);
		writeData(file);
		writeData(file);
		writeData(file);
	} catch(std::exception e) {
		fclose(file);
		throw;
	}

	printf("\nfile %s saved\n\n", filename);
	fclose(file);
}

void printMenu() {
	printf("-----------------------\n");
	printf("1: save file\n");
	printf("0: exit\n");
	printf("Input: ");
}

void userInput() {
	for(;;) {
		printMenu();
		int s;
		scanf("%d", &s);
		switch (s) {
			case 0: {
				return;
			}
			case 1: {
				try {
					saveFile("test.txt");
				} catch(MyError e) {
					fprintf(stderr, "error save file\n");
				}
				break;
			}
		}
	}
}

class MyObject {
public:
	MyObject() {
		throw MyError(-100);
	}

	~MyObject() { //destructor, don't throw exception !!
	}
};

int main() {

	userInput();

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}