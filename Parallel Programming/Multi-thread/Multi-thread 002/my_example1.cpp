#include "precompiledHeader.h"

void my_example1() {
	printf("\n========== my_example1 ========\n");

	auto t = GetCurrentThread();
	SetThreadAffinityMask(t, 1 << 3);

	int64_t resultCount = 0;
	MyTimer timer;

	for (int64_t i = 0; i < kBatchSize * kThreadCount; i++) {
		int64_t v = kPrimeStart + i;
		if (primeNumber(v)) {
			printf("prime: %lld\n", v);
			resultCount++;
		}
	}
	printf("done\n");
	printf("result count %lld\n", resultCount);
	timer.print();
}