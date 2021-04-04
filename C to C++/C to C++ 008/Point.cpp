#include "Point.h"

template<> 
void Point<float>::print() const {
	printf("Point %f, %f\n", x, y);
}

template<> 
void Point<int>::print() const {
	printf("Point %d, %d\n", x, y);
}

template<> 
void Point<double>::print() const {
	printf("Point %lf, %lf\n", x, y);
}

template<class T>
void Point<T>::func2() {
	printf("func2\n");
}

//---- The explicit instantiation ----
template Point<float>;
template Point<double>;