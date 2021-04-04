#include <stdio.h>
#include <conio.h>

#include "Rectangle.h"
#include "Circle.h"

int main() {
	
	Shape s(1);

	Circle c(2, 50);

	c.setColor(2);
	c.Shape::setColor(2);

	printf("=== Program Ended ===\n");
	printf("Press any to key to Exit !");
	_getch();
	return 0;
}