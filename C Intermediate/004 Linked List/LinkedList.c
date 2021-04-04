#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct StudentStruct {
	int id;
	char name[100];
	int age;
	char address[2000];

	struct StudentStruct* next;
};

typedef struct StudentStruct Student;

void Student_Init(Student* s) {
	s->id = 0;
	s->name[0] = 0;
	s->age = 0;
	s->address[0] = 0;
	s->next = NULL;
}

void Student_InitWithId(Student* s, int id, char* name) {
	Student_Init(s);
	s->id = id;
	strcpy(s->name, name);
}

Student* Student_NewWithId(int id, char* name) {
	Student* s = (Student*)malloc(sizeof(Student));
	Student_InitWithId(s, id, name);
	return s;
}

typedef struct {
	Student* head;
	Student* tail;
	int count;
} SList;

void SList_Init(SList* ls) {
	ls->head = NULL;
	ls->tail = NULL;
	ls->count = 0;
}

void SList_InsertToHead(SList* ls, Student* s) {
	assert(s->next == NULL);

	if (ls->head) {
		s->next = ls->head;
		ls->head = s;
	} else {
		ls->head = s;
		ls->tail = s;
	}
	ls->count++;
}

void SList_Insert(SList* ls, Student* s, Student* after) {
	if (!after) {
		SList_InsertToHead(ls, s);
	} else {
		s->next = after->next;

		if (!after->next)
			ls->tail = s;

		after->next = s;
		ls->count++;
	}
}

Student* SList_GetTail(SList* ls) {
	return ls->tail;
	/*
	for (Student* p = ls->head; p; p = p->next) {
		if (!p->next)
			return p;
	}
	return NULL;
	*/
}

void SList_Append(SList* ls, Student* s) {
	//Student* tail = SList_GetTail(ls);
	SList_Insert(ls, s, ls->tail);
}

Student* SList_PopHead(SList* ls) {
	Student* p = ls->head;
	if (!p)
		return NULL;

	ls->head = p->next;
	p->next = NULL;

	assert(ls->count > 0);
	ls->count--;
	return p;
}

void SList_Print(SList* ls) {
	printf("\nprint list count=%d:\n", ls->count);
	Student* p = ls->head;
	while (p) {
		printf("  %d: %s\n", p->id, p->name);
		p = p->next;
	}
	printf("\n");
}

Student* SList_FindById(SList* ls, int id) {
	for (Student* p = ls->head; p; p = p->next) {
		if (p->id == id)
			return p;
	}
	return NULL;
}

Student* SList_GetPrev(SList* ls, Student* s) {
	for (Student* p = ls->head; p; p = p->next) {
		if (p->next == s)
			return p;
	}
	return NULL;
}

void SList_Remove(SList* ls, Student* s) {
	printf("remove %s\n", s->name);

	Student* prev = SList_GetPrev(ls, s);
	if (prev) {
		prev->next = s->next;
		s->next = NULL;
	} else {
		ls->head = s->next;
		s->next = NULL;
	}

	ls->count--;
}

void SList_Release(SList* ls) {
	Student* p = ls->head;
	while (p) {
		Student* next = p->next;
		free(p);
		p = next;
	}
	ls->count = 0;
}

int main() {
	SList ls;
	SList_Init(&ls);

	Student* john = Student_NewWithId(10, "John");
	SList_Append(&ls, john);

	Student* mary = Student_NewWithId(44, "Mary");
	SList_Insert(&ls, mary, NULL);

	Student* tom = Student_NewWithId(6, "Tom");
	SList_Insert(&ls, tom, NULL);

	SList_Print(&ls);
	Student* bob = Student_NewWithId(36, "Bob");
	SList_Insert(&ls, bob, tom);

	SList_Print(&ls);

	SList_Release(&ls);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}