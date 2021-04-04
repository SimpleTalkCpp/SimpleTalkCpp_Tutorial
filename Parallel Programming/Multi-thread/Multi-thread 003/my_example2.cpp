#include "my_common.h"

class Example2 {
public:
	static const int N = 100000000;

	void run_int() {
		printf("==== run_int ====\n");

		int32_t a = 0;

		MyTimer timer;
		a = 0;

		std::thread thread0([&a](){
			for (int i = 0; i < N; i++) {
				a++;
			}
		});

		for (int i = 0; i < N; i++) {
			a++;
		}

		thread0.join();

		int32_t v = a;
		printf("a = %d <-- incorrect, %d expected\n", v, N * 2);
		timer.print();
	}

	void run_volatile_int() {
		printf("==== run_volatile_int ====\n");

		volatile int32_t a = 0;

		MyTimer timer;

		std::thread thread0([&a](){
			for (int i = 0; i < N; i++) {
				a++;
			}
		});

		for (int i = 0; i < N; i++) {
			a++;
		}

		thread0.join();

		int32_t v = a;
		printf("a = %d <-- incorrect, %d expected\n", v, N * 2);
		timer.print();
	}

	void run_mutex_int() {
		printf("==== run_mutex_int ====\n");

		using Lock = std::lock_guard<std::mutex>;

		int32_t a = 0;
		std::mutex mutex;

		MyTimer timer;

		std::thread thread0([&a, &mutex](){
			for (int i = 0; i < N; i++) {			
				Lock lock(mutex);
				a++;
			}
		});

		for (int i = 0; i < N; i++) {
			Lock lock(mutex);
			a++;
		}

		thread0.join();

		int32_t v = a;
		printf("a = %d\n", v);
		timer.print();
	}

	void run_interlock_add() {
		printf("==== run_interlock_add ====\n");
		volatile LONG a = 0;

		MyTimer timer;

		std::thread thread0([&a](){
			for (int i = 0; i < N; i++) {
				InterlockedAdd(&a, 1);
			}
		});

		for (int i = 0; i < N; i++) {
			InterlockedAdd(&a, 1);
		}

		thread0.join();

		int32_t v = a;
		printf("a = %d\n", v);
		timer.print();
	}

	void run_atomic_int() {
		printf("==== run_atomic_int ====\n");
		std::atomic_int32_t a = 0;

		MyTimer timer;

		std::thread thread0([&a](){
			for (int i = 0; i < N; i++) {
				a++;
			}
		});

		for (int i = 0; i < N; i++) {
			a++;
		}

		thread0.join();

		int32_t v = a;
		printf("a = %d\n", v);
		timer.print();
	}

	void run() {
		run_int();
		run_volatile_int();
		run_mutex_int();
		run_interlock_add();
		run_atomic_int();
	}
};

void my_example2() {
	Example2 ex;
	ex.run();
}