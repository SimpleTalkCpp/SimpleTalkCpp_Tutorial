#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct StudentStruct {	
	struct StudentStruct* prev;
	struct StudentStruct* next;
	void* list; //for validation

	int id;
	char name[100];
	int age;
	char address[2000];
};

typedef struct StudentStruct Student;

void Student_Init(Student* s) {
	s->id = 0;
	s->name[0] = 0;
	s->age = 0;
	s->address[0] = 0;
	s->prev = NULL;
	s->next = NULL;
	s->list = NULL;
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
} DList;


void DList_Init(DList* ls) {
	ls->head = NULL;
	ls->tail = NULL;
	ls->count = 0;
}

void DList_InsertToHead(DList* ls, Student* s) {
	assert(!s->prev && !s->next && !s->list);

	s->prev = NULL;

	if (ls->head) {
		s->next = ls->head;
		ls->head->prev = s;
	} else {
		ls->tail = s;
	}
	
	s->list = ls;

	ls->head = s;
	ls->count++;
}

void DList_Insert(DList* ls, Student* s, Student* after) {
	assert(!s->prev && !s->next && !s->list);

	if (!after) {
		DList_InsertToHead(ls, s);
		return;
	}

	s->prev = after; //1
	s->next = after->next; //2

	if (after->next) {
		after->next->prev = s; //3
	} else {
		ls->tail = s; //3
	}

	after->next = s; //4
	s->list = ls;
	ls->count++;
}

void DList_AppendToTail(DList* ls, Student* s) {	
	//TODO
	assert(0);
}

void DList_Append(DList* ls, Student* s, Student* before) {
	//TODO
	assert(0);
}

void DList_Remove(DList* ls, Student* s) {
	assert(s->list == ls);

	printf("remove %s\n", s->name);
	if (s->prev) {
		s->prev->next = s->next;
	} else {
		ls->head = s->next;
	}

	if (s->next) {
		s->next->prev = s->prev;
	} else {
		ls->tail = s->prev;
	}

	s->prev = s->next = NULL;
	ls->count--;
}

void DList_Print(DList* ls) {
	printf("\nprint list count=%d:\n", ls->count);
	Student* p = ls->head;
	while (p) {
		printf("  %d: %s\n", p->id, p->name);
		p = p->next;
	}
	printf("\n");
}

void DList_Release(DList* ls) {
	Student* next;
	for (Student* p = ls->head; p; p = next) {
		next = p->next;
		free(p);
	}
	ls->count = 0;
}

typedef DList StudentList;

int main() {
	DList ls;
	DList_Init(&ls);

	Student* john = Student_NewWithId(10, "John");
	DList_Insert(&ls, john, NULL);

	Student* mary = Student_NewWithId(44, "Mary");
	DList_Insert(&ls, mary, NULL);

	Student* tom = Student_NewWithId(6, "Tom");
	DList_Insert(&ls, tom, NULL);

	DList_Print(&ls);

	Student* bob = Student_NewWithId(36, "Bob");
	DList_Insert(&ls, bob, john);

	DList_Print(&ls);
	DList_Remove(&ls, bob);

	DList_Print(&ls);

	DList_Release(&ls);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}
