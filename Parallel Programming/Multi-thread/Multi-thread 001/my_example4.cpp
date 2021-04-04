#include "stdafx.h"

const int N = 1000000;

static int g_count = 0;

std::mutex g_mutex;

DWORD WINAPI my_example4_thread_func(void* param) {
	for (int i = 0; i < N; i++) {
		std::lock_guard<std::mutex> lock(g_mutex);

		g_count = g_count + 1;
	}
	return 0;
}

void my_example4() {
	printf("\n======== my_example4 =============\n");

	HANDLE my_thread = CreateThread(nullptr, 0, &my_example4_thread_func, nullptr, 0, nullptr);
	for (int i = 0; i < N; i++) {
		std::lock_guard<std::mutex> lock(g_mutex);

		g_count = g_count + 1;
	}

	WaitForSingleObject(my_thread, INFINITE);

	printf("g_count = %d\n", g_count);
}
