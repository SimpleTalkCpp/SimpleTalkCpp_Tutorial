#include "IntArray.h"

int main() {
//	int b[10]; // C Array

	IntArray a;

	a.init();
	a.resize(5);

	int n = a.size();

	int i ;
	int& r = i;
	int* p = &i;

	for (int i = 0; i < 5; i++) {
//		a.data[i] = i;
		//a.data(i) = i;
		a[i] = i;
	}

	a.print();

	a.release();

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();

	return 0;
}