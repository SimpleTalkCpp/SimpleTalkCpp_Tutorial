#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
	char name[100];
	int age;
} Student;

void Student_Init(Student* s) {
	s->name[0] = 0;
	s->age = 0;
}

void Student_InitWithName(Student* s, char* name, int age) {
	strcpy(s->name, name);
	s->age = age;
}

void WriteInt(FILE* f, int v) {
	fwrite(&v, sizeof(v), 1, f);
}

void WriteChar(FILE* f, char c) {
	fwrite(&c, sizeof(c), 1, f);
}

void Student_WriteFile(Student* s, FILE* f) {
	//fprintf(f, "%s %d\n", s->name, s->age);
	//int ret = fwrite(s, sizeof(Student), 1, f);
	//if (ret !=1) {
	//	assert(0);
	//	abort();
	//}

	int nameLen = strlen(s->name);
	fwrite(s->name, nameLen + 1, 1, f);
	WriteInt(f, s->age);
}

void ReadString(FILE* f, char* buf, int buf_size) {
	int size = 0;

	for(;;) {
		int c = fgetc(f);
		if (c == EOF)
			break;

		if (c == 0)
			break;

		if (size >= buf_size) {
			//string is too big
			assert(0);
			abort();
		}

		buf[size] = c;
		size++;
	}

	if (size >= buf_size) {
		//string is too big
		assert(0);
		abort();
	}
	buf[size] = 0;
}

void Student_ReadFile(Student* s, FILE* f) {
	//int ret = fread(s, sizeof(Student), 1, f);
	//if (ret !=1) {
	//	assert(0);
	//	abort();
	//}

	ReadString(f, s->name, sizeof(s->name));
}

int BEtoLE(int v) {
	uint8_t* c = (uint8_t*)&v;
	uint32_t o = (uint32_t)c[3] << 24
	           | (uint32_t)c[2] << 16
			   | (uint32_t)c[1] << 8
			   | (uint32_t)c[0];

	return o;
}

void WriteList(Student* arr, int n) {
	FILE* f = fopen("StudentList", "wb");
	if (!f) {
		printf("cannot open file");
		assert(0);
		abort();
	}

	int ret = fwrite(&n, sizeof(n), 1, f);
	if (ret !=1) {
		assert(0);
		abort();
	}

	for (int i = 0; i < n; i++) {
		Student_WriteFile(&arr[i], f);
	}
	fclose(f);
}

int ReadList(Student* arr, int buffer_size) {
	FILE* f = fopen("StudentList", "rb");
	if (!f) {
		printf("cannot open file");
		assert(0);
		abort();
	}

	int n;
	int ret = fread(&n, sizeof(n), 1, f);
	if (ret !=1) {
		assert(0);
		abort();
	}

	for (int i =0; i<n; i++) {
		Student_ReadFile(&arr[i], f);
	}
	fclose(f);
	return n;
}

void PrintList(Student* arr, int n) {
	printf("print...\n");
	for (int i=0; i<n; i++) {
		Student* s = &arr[i];
		printf("  %s %d\n", s->name, s->age);
	}
	printf("\n");
}

int main() {
	printf("sizeof = %d\n", sizeof(Student));

	Student s[2];
	int n = 2;

	Student a;

	Student_Init(&s[0]);
	Student_Init(&s[1]);
//	ReadList(s, 2);


	Student_InitWithName(&s[0], "John", 0x12345678);
	Student_InitWithName(&s[1], "Mary", 11);
	WriteList(s, 2);

	PrintList(s, 2);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}