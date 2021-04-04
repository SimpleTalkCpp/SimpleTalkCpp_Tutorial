#pragma once

#include <stdio.h>
#include <cassert>

enum ShapeType {
	ShapeType_Unknown,
	ShapeType_Circle,
	ShapeType_Rectange,
};

// abstract class
class Shape {
public:
	Shape(int id);

	//virtual destructor
	virtual ~Shape() {
		printf("~Shape\n");
	}

	int id() { return m_id; }
	virtual void print();

//	virtual float calcArea() = 0; //pure virtual function
	virtual float calcArea() { return 0; }

protected:
	int m_id;
	ShapeType m_type;

};
