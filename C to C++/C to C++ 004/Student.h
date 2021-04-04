#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

class Student {
public:
	Student();
	Student(char* name, int age = 0);
	~Student();

	void print(/* Student* this */);
	void setName(char* name);

	//static function
	static void testing(/* no this !!!  */);

private:
	void _init(char* name, int age);

	static int s_instance_count;
	char m_name[50];
	int m_age;
};
