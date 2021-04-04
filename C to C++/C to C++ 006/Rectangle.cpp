#include "Rectangle.h"
#include "Circle.h"

Rectangle::Rectangle(int id, float w, float h) 
: Shape(id)
{
	m_x = 0;
	m_y = 0;
	m_width = w;
	m_height = h;

	m_type = ShapeType_Rectange;
}

void Rectangle::crossFunc(Circle& c) {
	printf("Rectangle::crossFunc Circle %d\n", c.id());
}

void Rectangle::print() {
	Shape::print();
	printf("  Rect w=%f h=%f\n", m_width, m_height);
}