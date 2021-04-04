#include <iostream>
#include <typeinfo>

#include "Shape.h"

void func(Shape& s) {
	std::cout << "-------------\n";
//	std::cout << typeid(s).name() << "\n";
//	if (auto* c = dynamic_cast<Circle*>(&s)) {
//		std::cout << "radius=" << c->radius << "\n";
//	}

	auto* ti = my_typeof(s);
	if (!ti) {
		std::cout << "unknown type\n";
		return;
	}
	
	std::cout << ti->name << "\n";

	auto en = ti->fields();

	//for (auto it = en.begin(); it != en.end(); ++it)
	for (auto& f : ti->fields()) {
		std::cout << f.name << " type=" << f.fieldType->name << " offset=" << f.offset << "\n";
	}

	Circle* c = my_cast<Circle>(&s);
	if (c) {
		std::cout << "radius=" << c->radius << "\n";
	}
}

int main() {
	auto* mgr = TypeManager::instance();
	mgr->registerType("Rect", my_typeof<Rect>());

	auto* ti = TypeManager::instance()->find("Rect");
	MyObject* obj = ti->createObject();
	

	Shape shape;
	func(shape);

	Circle circle;
	func(circle);

	Rect rect;
	func(rect);
}