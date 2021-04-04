#include <stdio.h>
#include <conio.h>
#include <cmath>


// constant
//#define N (5 + 1)

// macro as function
//#define FIND_MIN(a, b) (a < b ? a : b)

struct Point {
};

inline int find_min(int a, int b) {
	//if ( a < b ) 
	//	return a;
	//else
	//	return b;
	return a < b ? a : b;
}

const int N = 5 + 1;

int main() {

	int c = find_min(1,2);
	int d = FIND_MIN(1,2);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}