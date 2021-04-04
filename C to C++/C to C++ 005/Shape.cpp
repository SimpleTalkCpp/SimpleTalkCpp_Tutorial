#include "Shape.h"

Shape::Shape(int color_) {
	color = color_;
}

void Shape::setColor(int c) {
	printf("Shape::setColor\n");
	color = c;
}

