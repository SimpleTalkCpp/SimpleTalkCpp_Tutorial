#include "stdafx.h"

const int N = 1000000;

static int g_count = 0;

class MyLock : public MyNonCopyable {
public:
	MyLock() {
		InitializeCriticalSection(&m_cs);
	}

	~MyLock() {
		DeleteCriticalSection(&m_cs);
	}

	void lock() {
		EnterCriticalSection(&m_cs);
	}

	void unlock() {
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
};

MyLock g_lock;

//RAII
class MyLockGuard : public MyNonCopyable {
public:
	MyLockGuard(MyLock& lock) {
		p = &lock;
		lock.lock();
	}

	~MyLockGuard() {
		p->unlock();
	}

private:
	MyLock* p;
};


DWORD WINAPI my_example3_thread_func(void* param) {
	for (int i = 0; i < N * 2; i++) {
//		g_lock.lock();

		MyLockGuard g(g_lock);

		g_count = g_count + 1;

//		g_lock.unlock();
	}
	return 0;
}

void my_example3() {
	printf("\n======== my_example3 =============\n");

	HANDLE my_thread = CreateThread(nullptr, 0, &my_example3_thread_func, nullptr, 0, nullptr);
	for (int i = 0; i < N; i++) {
		g_lock.lock();

		g_count = g_count + 1;

		g_lock.unlock();
	}

	WaitForSingleObject(my_thread, INFINITE);

	printf("g_count = %d\n", g_count);
}
