#pragma once

#include "Shape.h"

// Derived / Sub Class  :  Base / Super class
class Circle : public Shape {
	typedef Shape Base;

public:
	float x,y;
	float radius;

	Circle(int color_, float radius);

	void setRadius(float r);
	void setColor(int c);
};