#include "Circle.h"

Circle::Circle(int color_, float radius) 
	: Base(color_)
	, x(0)
	, y(0)
	, radius(0)
{
}

void Circle::setRadius(float r) {
	radius = r;
}

//override Shape::setColor
void Circle::setColor(int c) {
	Base::setColor(c);
	printf("Circle::setColor\n");
	color = c + 1;
}
