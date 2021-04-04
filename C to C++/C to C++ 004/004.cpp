#define _CRT_SECURE_NO_WARNINGS

#include "Student.h"

static void HelperFunc() {
	printf("main helper");
}

int main() {
	//class (type)   a object (instance)
	Student a("John");

	a.print();
	a.print();
	a.print();

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();

	return 0;
}