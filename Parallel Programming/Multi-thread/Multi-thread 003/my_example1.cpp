#include "my_common.h"

class Example1 {
public:
	using Lock = std::unique_lock<std::mutex>;

	static void my_sleep() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	class BankAccount {
	public:
		void transfer(BankAccount& to, int amount) {
			//doTransfer_DeadLock(to, amount);
			//doTransfer_LockInOrder(to, amount);
			doTransfer_TryLock(to, amount);
		}

		void doTransfer_DeadLock(BankAccount& to, int amount) {
			printf("Thread %08X: start transfer %p -> %p\n", myGetThreadId(), this, &to);

			printf("Thread %08X: lock %p\n", myGetThreadId(), this);
			Lock lockFrom(_mutex);

			my_sleep();

			printf("Thread %08X: lock %p\n", myGetThreadId(), &to);
			Lock lockTo(to._mutex);

			my_sleep();
			_balance -= amount;
			to._balance += amount;

			printf("Thread %08X: end transfer %p -> %p\n", myGetThreadId(), this, &to);
		}

		void doTransfer_LockInOrder(BankAccount& to, int amount) {
			printf("Thread %08X: start transfer %p -> %p\n", myGetThreadId(), this, &to);

			Lock lockFrom(_mutex, std::defer_lock);
			my_sleep();
			Lock lockTo(to._mutex, std::defer_lock);

			if (this < &to) {
				printf("Thread %08X: lock %p\n", myGetThreadId(), this);
				lockFrom.lock(); // <--- lockA first

				my_sleep();

				printf("Thread %08X: lock %p\n", myGetThreadId(), &to);
				lockTo.lock();

			} else {
				printf("Thread %08X: lock %p\n", myGetThreadId(), &to);
				lockTo.lock(); // <-- lockB first

				my_sleep();

				printf("Thread %08X: lock %p\n", myGetThreadId(), this);
				lockFrom.lock();
			}

			my_sleep();
			_balance -= amount;
			to._balance += amount;

			printf("Thread %08X: end transfer %p -> %p\n", myGetThreadId(), this, &to);
		}

		void doTransfer_TryLock(BankAccount& to, int amount) {
			printf("Thread %08X: start transfer %p -> %p\n", myGetThreadId(), this, &to);

			for(;;) {
				printf("Thread %08X: lock %p\n", myGetThreadId(), this);
				Lock lockFrom(_mutex);

				my_sleep();

				printf("Thread %08X: try lock %p\n", myGetThreadId(), this);
				Lock lockTo(to._mutex, std::try_to_lock);

				if (!lockTo.owns_lock()) {
					printf("Thread %08X: unlock %p\n", myGetThreadId(), this);
					lockFrom.unlock(); // <--- unlock before sleep
					my_sleep();
					continue;
				}

				my_sleep();
				_balance -= amount;
				to._balance += amount;

				printf("Thread %08X: end transfer %p -> %p\n", myGetThreadId(), this, &to);
				break;
			}
		}

		int balance() {
			Lock lock(_mutex);
			return _balance;
		}

	private:
		std::mutex _mutex;
		int _balance = 1000;
	};

	void run() {
		{
			printf("Thread %08X: main thread\n", myGetThreadId());
		}

		std::thread thread0([this](){
			printf("Thread %08X: thread0 start\n", myGetThreadId());
			accountA.transfer(accountB, 10);
		});

		accountB.transfer(accountA, 1);

		thread0.join();
	}

	BankAccount accountA;
	BankAccount accountB;
};

void my_example1() {
	Example1 ex;
	ex.run();
}