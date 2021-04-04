#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"

Shape::Shape(int id) {
	m_id = id;
	m_type = ShapeType_Unknown;
}

void Shape::print() {
	printf("shape id = %d, type = %d\n", m_id, m_type);

	//dispatch by type
	/*
	switch (m_type)
	{
	case ShapeType_Unknown:
		break;
	case ShapeType_Circle:
	{
		Circle* p = static_cast<Circle*>(this);
		p->print();
		break;
	}
	case ShapeType_Rectange:
	{
		Rectangle* p = static_cast<Rectangle*>(this);
		p->print();
		break;
	}
	default:
		break;
	}
	*/
}
