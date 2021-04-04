#pragma once

#include "Shape.h"

class Rectangle;

// Derived / Sub Class  :  Base / Super class
class Circle : public Shape {
	typedef Shape Base;

public:
	Circle(int id, float radius);

	~Circle() {
		printf("~Circle\n");
	}

	void setRadius(float r);

	void crossFunc(Rectangle& c);

	void print();

	virtual float calcArea() { 
		return 2 * 3.1416f * m_radius * m_radius; 
	}

private:
	float m_x, m_y;
	float m_radius;

};