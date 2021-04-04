#pragma once

#include "Reflection.h"

class Shape : public MyObject {
	MY_TYPE(Shape, MyObject)
public:
	float x = 0;
	float y = 0;
};

class Circle : public Shape {
	MY_TYPE(Circle, Shape)
public:
	double radius = 0;
};

class Rect : public Shape {
	MY_TYPE(Rect, Shape)
public:
	float width = 0;
	float height = 0;
};

