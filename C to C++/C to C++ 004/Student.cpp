#include "Student.h"

int Student::s_instance_count = 0;

//function for this .cpp only
static void HelperFunc() {
	printf("Student Helper");
}

Student::Student() {
	_init("", 0);
}

Student::Student(char* name, int age) {
	_init(name, age);
}

void Student::_init(char* name, int age) {
	s_instance_count++;

	::printf(" init %s\n", name);

	strcpy(m_name, name);
	m_age = age;
}

Student::~Student() {
	::printf(" release %s\n", m_name);
	s_instance_count--;
}

void Student::print(/* Student* this */) {
	static int i = 0; //function static var
	i++;

	::printf("%d %s\n", i, m_name);
}

//static function
void Student::testing(/* no this!!!  */) {
	printf("Testing");
}

void Student::setName(char* name) {
	strcpy(m_name, name);
}
