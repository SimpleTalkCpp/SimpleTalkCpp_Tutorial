#include <stdio.h>
#include <conio.h>

class MyObject {
public:
	int i;
	int j;

	mutable int value; //mutable even const
	mutable bool dirty;

	void set(int i_, int j_) {
		i = i_;
		j = j_;
		dirty = true;
	}
	
	//lazy eval
	int getValue() { 
		if (dirty) {
			value = i + j;
			dirty = false;
		}

		printf("mutable getValue\n");
		i++;
		return i; 
	}
	
	int getValue() const { 
		if (dirty) {
			//MyObject* m = const_cast<MyObject*>(this); // mutable_this
			//m->value = i + j;
			//m->dirty = false;

			value = i + j;
			dirty = false;
		}

		printf("const getValue\n");
		return i;
	}
};

void print_func(const MyObject& a) {
	printf("%d", a.getValue());
}

int main() {
	const int i = 10;
	//i = 11; // cannot change const

	MyObject a; // mutable
	a.getValue();
	a.i = 10;

	print_func(a);

	const MyObject * const p = &a; //const pointer to const value
	//p->i = 11; // cannot change const member

	MyObject b;
//	p = &b;

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}