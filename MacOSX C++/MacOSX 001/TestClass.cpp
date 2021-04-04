#include "TestClass.h"

TestClass::TestClass() {
	m_date = [NSDate date];
}

void TestClass::testPrint() {
	auto desc = [m_date description];
	const char* sz = [desc UTF8String];
	std::cout << "Date is " << sz << "\n";
}
