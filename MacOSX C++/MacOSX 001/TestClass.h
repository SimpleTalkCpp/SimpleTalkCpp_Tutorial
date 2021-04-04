#pragma once

#ifndef __OBJC__
	#error "Objective-C++ required"
#else

#include <iostream>

#import <Foundation/Foundation.h>

class TestClass {
public:

	TestClass();

	void testPrint();

private:
	NSDate* m_date = nil;
};

#endif
