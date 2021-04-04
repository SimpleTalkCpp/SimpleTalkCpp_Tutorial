#pragma once

#include <stdio.h>

template<class T>
class Point {
public:
	T x, y;

	Point(T x_, T y_)
		: x(x_)
		, y(y_)
	{}

	void print() const;
	Point operator+(const Point& rhs) const;
	void operator+=(const Point& rhs);

	template<class Y>
	void func();

	void func2();
};

typedef Point<float>  Point2f;
typedef Point<double> Point2d;

//C++ template meta programming

/*
inline
Point operator(const Point& a, const Point& b) {
}
*/

template<class T> 
inline Point<T> Point<T>::operator+(const Point& rhs) const {
	return Point(x + rhs.x, y + rhs.y);
}

template<class T> 
inline void Point<T>::operator+=(const Point& rhs) {
	x += rhs.x;
	y += rhs.y;
}


template<class T> // template class
template<class Y> // tempalte member function
inline void Point<T>::func() {
	printf("func\n");
}
