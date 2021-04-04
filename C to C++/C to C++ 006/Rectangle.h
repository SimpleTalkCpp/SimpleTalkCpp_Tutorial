#pragma once

#include "Shape.h"

class Circle; //forward declare

class Rectangle : public Shape {
public:
	float m_x, m_y;
	float m_width, m_height;

	Rectangle(int id, float w, float h);
	~Rectangle() {
		printf("~Rectangle\n");
	}

	void crossFunc(Circle& c);

	void print();

	virtual float calcArea() { return m_width * m_height; }

friend class Circle;
protected:
	int protected_member;
};