#include "stdafx.h"

const int N = 1000;

DWORD WINAPI my_example1_thread_func(void* param) {
	for (int i = 0; i < N; i++) {
		printf("+");
		//std::cout << "thread " << i << "\n";
	}

	return 0;
}

void my_example1() {
	printf("\n======== my_example1 =============\n");

	HANDLE my_thread = CreateThread(nullptr, 0, &my_example1_thread_func, nullptr, 0, nullptr);
	for (int i = 0; i < N; i++) {
		printf(".");
		//std::cout << "main " << i << "\n";
	}

	WaitForSingleObject(my_thread, INFINITE);
}