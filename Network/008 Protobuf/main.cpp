#include <stdio.h>
#include <iostream>
#include <conio.h>

#include "proto/test.pb.h"


#ifdef _DEBUG
	#pragma comment(lib,"libprotobuf-lited.lib")
#else
	#pragma comment(lib,"libprotobuf-lite.lib")
#endif

int main() {

	std::string tmp;

	{
		tutorial::Person p;
		p.set_id(100);
		p.set_name("test");
		auto* phone = p.add_phones();
		phone->set_number("111111");
		phone->set_type(tutorial::Person_PhoneType_HOME);

		p.SerializeToString(&tmp);
	}

	{
		tutorial::Person a;
		a.ParseFromString(tmp);
		printf("id=%d, name=%s\n", a.id(), a.name().c_str());
	}

	_getch();
}