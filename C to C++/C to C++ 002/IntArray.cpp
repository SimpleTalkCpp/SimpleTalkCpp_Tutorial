#include <stdlib.h>
#include "IntArray.h"

void IntArray::init(/* IntArray* this  */) {
	m_data = NULL;
	m_size = 0;
}

void IntArray::resize(/* IntArray* this  */ int new_size) {
	int* p = (int*)malloc(sizeof(int) * new_size);

	if (m_data) {
		int copy_count = find_min(m_size, new_size);
		for (int i = 0; i < copy_count; i++) {
			p[i] = m_data[i];
		}
		free(m_data);
	}
	m_data = p;
	m_size = new_size;
}

void IntArray::release() {
	if (m_data) {
		free(m_data);
	}
	m_size = 0;
}


void IntArray::print() {
	for (int i = 0; i < m_size; i++) {
		printf("%d ", m_data[i]);
	}
	printf("\n");
}
