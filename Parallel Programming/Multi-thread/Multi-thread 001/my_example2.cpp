#include "stdafx.h"

const int N = 1000000;
static int g_count = 0;

DWORD WINAPI my_example2_thread_func(void* param) {
	for (int i = 0; i < N * 2; i++) {
		g_count = g_count + 1;
	}

	return 0;
}

void my_example2() {
	printf("\n======== my_example2 =============\n");

	HANDLE my_thread = CreateThread(nullptr, 0, &my_example2_thread_func, nullptr, 0, nullptr);
	for (int i = 0; i < N; i++) {
		g_count = g_count + 1;
	}

	WaitForSingleObject(my_thread, INFINITE);

	printf("g_count = %d is incorrect !!! \n", g_count);
}
