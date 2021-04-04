#include "precompiledHeader.h"

class Example3 {
public:

	class MyRequest {
	public:
		using Lock = std::unique_lock<std::mutex>;

		void init(int64_t start, int64_t count) {
			_start = start;
			_count = count;
			_current = start;
		}

		int64_t getNext() {
			Lock lock(_mutex);
			if (_current >= _start + _count)
				return 0;

			auto ret = _current;
			_current++;
			return ret;
		}

		void addResult(int64_t v) {
			Lock lock(_mutex);
			_primeNumbers.push_back(v);
		}

		void finish() {
			Lock lock(_mutex);

			printf("result count: %lld\n", _primeNumbers.size());
		}

	private:
		int64_t _start = 0;
		int64_t _count = 0;
		int64_t _current = 0;

		std::mutex _mutex;
		std::vector<int64_t> _primeNumbers;
	};

	class MyThread {
	public:
		~MyThread() {
			join();
		}

		void join() {
			if (_thread) {
				WaitForSingleObject(_thread, INFINITE);
			}
		}

		static DWORD WINAPI _proc(void* p) {
			reinterpret_cast<MyThread*>(p)->onProc();
			return 0;
		}

		void onProc() {
			printf("Thread %d: start\n", _id);
			SetThreadAffinityMask(_thread, 1LL << _id);

			for (;;) {
				int64_t v = _req->getNext();
				if (v == 0) {
					break;
				}

				if (primeNumber(v)) {
					printf("Thread %d: prime %lld\n", _id, v);
					_req->addResult(v);
				}
			}

			printf("Thread %d: ended\n", _id);
		}

		void run(MyRequest& req, int id) {
			_req = &req;
			_id = id;
			_thread = CreateThread(nullptr, 0, &_proc, this, 0, nullptr);
		}

		MyRequest* _req = nullptr;
		HANDLE  _thread = nullptr;
		int     _id = 0;
	};

	static void run() {
		printf("\n========== my_example3 ========\n");

		MyRequest req;
		MyTimer timer;

		MyThread threads[kThreadCount];

		req.init(kPrimeStart, kThreadCount * kBatchSize);

		for (int i = 0; i < kThreadCount; i++) {
			threads[i].run(req, i);
		}

		for (int64_t i = 0; i < kThreadCount; i++) {
			threads[i].join();
		}

		printf("done\n");
		timer.print();
		req.finish();
	}

}; // Example3

void my_example3() {
	Example3::run();
}

