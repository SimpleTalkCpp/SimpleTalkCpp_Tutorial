#include "precompiledHeader.h"

class Example2 {
public:

	class MyRequest {
	public:
		using Lock = std::unique_lock<std::mutex>;

		void addResult(int64_t v) {
			Lock lock(_mutex);
			_primeNumbers.push_back(v);
		}

		void finish() {
			Lock lock(_mutex);
			printf("result count: %lld\n", _primeNumbers.size());

			std::sort(_primeNumbers.begin(), _primeNumbers.end());
			for (auto& e : _primeNumbers) {
				printf("prime: %lld\n", e);
			}
		}

	private:
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
			printf("Thread %d: start %lld %lld\n", _id, _start, _batch);
			SetThreadAffinityMask(_thread, 1LL << _id);

			for (int64_t i = 0; i < _batch; i++) {
				int64_t v = _start + i;
				if (primeNumber(v)) {
					printf("Thread %d: prime %lld\n", _id, v);
					_req->addResult(v);
				}
			}

			printf("Thread %d: ended\n", _id);
		}

		void run(MyRequest& req, int id, int64_t start, int64_t batch) {
			_req = &req;
			_id = id;
			_start = start;
			_batch = batch;
			_thread = CreateThread(nullptr, 0, &_proc, this, 0, nullptr);
		}

		MyRequest* _req = nullptr;
		HANDLE  _thread = nullptr;
		int     _id = 0;
		int64_t _start = 0;
		int64_t _batch = 0;
	};

	static void run() {
		printf("\n========== my_example2 ========\n");

		MyRequest req;
		MyTimer timer;

		MyThread threads[kThreadCount];

		for (int i = 0; i < kThreadCount; i++) {
			int64_t start = kPrimeStart + i * kBatchSize;
			threads[i].run(req, i, start, kBatchSize);
		}

		for (int64_t i = 0; i < kThreadCount; i++) {
			threads[i].join();
		}

		printf("done\n");
		timer.print();
		req.finish();
	}

}; // Example2

void my_example2() {
	Example2::run();
}