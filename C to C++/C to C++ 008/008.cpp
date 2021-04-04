#include <stdio.h>
#include <conio.h>

#include "Point.h"

//#define add(A, B) ((A) + (B))

//template function
template<class T>
inline T add(T a, T b) {
	return a + b;
}

class MyObject {
public:
	int value;
	bool operator< (const MyObject& rhs) const {
		return value < rhs.value;
	}
};

template<class T>
inline T find_min(const T& a, const T& b) {

	return a < b ? a : b;
}

int main() {
	MyObject o2;
	MyObject o3 = find_min<MyObject>(MyObject(), o2);

	int i = find_min(1, 2);

	float f = add<float>(1,2);
	double d = add<double>(1,2);

	//template parameter reduation 

	
	Point2f a(1,2);
	Point2f b(3,4);

//	Point2f c = a.operator+(b);
	Point2f c = a + b;
	c.print();

	c.func2();

	c += Point2f(5,6);
	
	Point2d e(4,5);
	e.print();

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}