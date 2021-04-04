#include "pch.h"

#include "my_common.h"

enum class ShapeType : int16_t {
	Shape,
	Rectangle,
	Circle,
};

class Shape {
public:
	virtual ~Shape() {}

	float x;
	float y;
};

class Rectangle : public Shape {
public:
	float width  = 0;
	float height = 0;
};

class Circle : public Shape {
public:
	float radius = 0;
};

void func(Shape* s) {
	//RTTI
	std::cout << typeid(*s).name() << "\n";
	
	if (typeid(*s) == typeid(Rectangle)) {
		auto* p = static_cast<Rectangle*>(s);
		p->width = 10;
	}

	if (auto* p = dynamic_cast<Rectangle*>(s)) {
		p->width = 10;
	}
}

void c_call_cpp_func() {
	std::cout << "c -> cpp\n";
}

int main() {
	cpp_call_c_func();

	Rectangle rect;
	func(&rect);

	Circle circle;
	func(&circle);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}