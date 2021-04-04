#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
	int id;
	char name[100];
	int age;
	char address[2000];
} Student;

void Student_Init(Student* s) {
	s->id = 0;
	s->name[0] = 0;
	s->age = 0;
	s->address[0] = 0;
}

void Student_InitWithId(Student* s, int id, char* name) {
	Student_Init(s);
	s->id = id;
	strcpy(s->name, name);
}

Student* Student_NewWithId(int id, char* name) {
	Student* s = (Student*) malloc(sizeof(Student));
	Student_InitWithId(s, id, name);
	return s;
}

typedef struct {
	Student* data;
	int size;
	int capacity;
} MyArray;

void MyArray_Resize(MyArray* a, int new_size);


void MyArray_Init(MyArray* a) {
	a->data = NULL;
	a->size = 0;
	a->capacity = 0;
}

void MyArray_Release(MyArray* a) {
	MyArray_Resize(a, 0);
	if (a->data) {
		free(a->data);
	}
	a->data = NULL;
	a->size = 0;
	a->capacity = 0;
}

void MyArray_Print(MyArray*a) {
	printf("print...\n");
	for (int i = 0; i < a->size; i++) {
		Student* s = &a->data[i];
		printf("  id=%d, name=[%s]\n", s->id, s->name);
	}
	printf("\n");
}

int round_pow2(int v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

void MyArray_ResizeCapacity(MyArray* a, int new_cap) {
	assert(new_cap > 0);

	new_cap = round_pow2(new_cap);
	printf("resize_cap %d %d\n", a->capacity, new_cap);

	Student* new_data = malloc(new_cap * sizeof(Student));
	if (!new_data) {
		printf("cannot allocate memory");
		abort();
		return;
	}

	for (int i = 0; i < a->size; i++) {
		new_data[i] = a->data[i];
	}

	if (a->data) {
		free(a->data);
	}

	a->data = new_data;
	a->capacity = new_cap;
}

void MyArray_Resize(MyArray* a, int new_size) {
	assert(new_size >= 0);

	if (new_size == a->size) {
		return;
	}

	if (a->capacity < new_size) {
		MyArray_ResizeCapacity(a, new_size);
	}

	a->size = new_size;
}

void MyArray_Append(MyArray* a, Student* value) {
	printf("append %d\n", value->id);

	MyArray_Resize(a, a->size + 1);
	a->data[a->size - 1] = *value;
}

void MyArray_Remove(MyArray* a, int index) {
	assert(index >= 0 && index < a->size);

	printf("remove %d\n", index);

	for (int i = index; i < a->size - 1; i++) {
		a->data[i] = a->data[i + 1];
	}
	a->size--;
}

int main() {
	MyArray a;
	MyArray_Init(&a);

	Student* john = Student_NewWithId(10, "John");
	MyArray_Append(&a, john);

	Student* mary = Student_NewWithId(44, "Mary");
	MyArray_Append(&a, mary);

	MyArray_Print(&a);
	MyArray_Resize(&a, 1);

	MyArray_Print(&a);
	MyArray_Release(&a);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}