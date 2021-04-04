#include <stdio.h>
#include <conio.h>

#include "Rectangle.h"
#include "Circle.h"

class ShapeList {
public:
	static const int kMaxSize = 10;
//	enum { kMaxSize = 10 };

	ShapeList() {
		m_size = 0;
	}
	
	~ShapeList() {
		for (int i=0; i<m_size; i++) {
			printf("-- delete -- %p\n", m_shape[i]);
			delete m_shape[i]; // call destructor, free()
		}
	}

	void add(Shape* s) {
		assert(m_size < kMaxSize);

		m_shape[m_size] = s;
		m_size++;
	}

	void print() {
		for (int i=0; i<m_size; i++) {
			m_shape[i]->print();
		}
	}

private:
	Shape* m_shape[kMaxSize];
	int m_size;
};

void my_test() {

	ShapeList list;
	list.add(new Circle(1, 5));
	list.add(new Rectangle(2, 5, 4));
	list.add(new Rectangle(3, 3, 2));

	list.print();
}

int main() {
	my_test();

	{
		/*
		// cross reference
		Circle c(11, 50);
		Rectangle rc(22, 10, 10);

		rc.crossFunc(c);
		c.crossFunc(rc);
		*/
	}

	Circle a(3,11);
	Circle c(1,10);
	Shape s(2);

//	Shape s; //cannot instantiante abstract class

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}