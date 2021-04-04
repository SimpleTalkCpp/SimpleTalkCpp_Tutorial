#include "Circle.h"
#include "Rectangle.h"

Circle::Circle(int id, float radius) 
	: Base(id)
	, m_x(0)
	, m_y(0)
	, m_radius(radius)
{
	m_type = ShapeType_Circle;
}

void Circle::setRadius(float r) {
	m_radius = r;
}

void Circle::crossFunc(Rectangle& rc) {
	rc.protected_member = 10;
	printf("Circle::croosFunc Rect %d\n", rc.id());
}

void Circle::print() {
	Shape::print();
	printf("  Circle radius = %f\n", m_radius);
}