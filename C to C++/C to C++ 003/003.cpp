#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

class Student {
public:
	Student() {
		_init("", 0);
	}

	Student(char* name, int age = 0) {
		_init(name, age);
	}

	~Student() {
		::printf(" release %s\n", m_name);		
	}

	void print() {
		::printf("%s\n", m_name);
	}

	void setName(char* name) {
		strcpy(m_name, name);
	}

private:
	void _init(char* name, int age) {
		::printf(" init %s\n", name);

		strcpy(m_name, name);
		m_age = age;
	}

	char m_name[50];
	int m_age;
};

int main() {
	int n = 10;
	Student* p = new Student[n]; // new array of objects

	//Student* p = (Student*) malloc(sizeof(Student) * n);
	//for (int i = 0; i < n; i++) {
	//	p[i].constructor();
	//}


	//for (int i = 0; i < n; i++) {
	//	p[i].destructor();
	//}
	// free(p);

	delete[] p;
	p = NULL;

	//------------------------

	Student* john = new Student("John"); // new object
	delete john;
	john = NULL;

	//----------------------------
	Student a("John");
	Student b("Mary", 11);
	Student c;

	a.print();

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();

	return 0;
}