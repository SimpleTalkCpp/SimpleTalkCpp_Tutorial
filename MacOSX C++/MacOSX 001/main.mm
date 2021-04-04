#include <iostream>
#include "TestClass.h"

int main(int argc, const char * argv[]) {
	// insert code here...
	std::cout << "Hello, World!\n";
	

	NSString* s = [NSString stringWithUTF8String:"My Log Message !!"];
	NSLog(@"%@", s);
	
	
	TestClass c;
	c.testPrint();
	
	return 0;
}
