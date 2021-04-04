#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cassert>

inline int find_min(int a, int b) {
	return a < b ? a : b;
}

class IntArray {
	int  m_size;
	int* m_data;

public:
	void init();
	void release();

	int  size() { return m_size;	}
	void resize(/* IntArray* this  */ int new_size);

	void print();

	int& data(int index) { return m_data[index]; }

	int& operator[](int index);
};

inline int& IntArray::operator[](int index) {
	assert(index >= 0 && index < m_size);
	return m_data[index];
}