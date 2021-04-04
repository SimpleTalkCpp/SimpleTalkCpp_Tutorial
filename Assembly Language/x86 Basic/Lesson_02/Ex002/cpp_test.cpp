#include <stdio.h>
#include <conio.h>

extern "C" {

extern void my_asm_func();

void my_hello() {
	printf("Hello World\n");
}

void my_print_int(int v0, int v1, int v2) {
	printf("my_print_int(%d, %d, %d)\n", v0, v1, v2);
}

void __stdcall my_print_int_stdcall(int v0, int v1, int v2) {
	printf("my_print_int_stdcall(%d, %d, %d)\n", v0, v1, v2);
}

// Obsoleted by Visual C++
//void __pascal my_print_int_pascal(int v0, int v1, int v2) {
//	printf("my_print_int_pascal(%d, %d, %d)\n", v0, v1, v2);
//}

void my_print_string(const char* pStr) {
	printf("my_print_string(%s)\n", pStr);
}

int main() {
	my_asm_func();

	printf("\n===== Program Ended =====\n  Press any key to exit !!\n");
	_getch();
}

}