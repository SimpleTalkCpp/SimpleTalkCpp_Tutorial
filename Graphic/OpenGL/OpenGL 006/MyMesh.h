#pragma once

#include "MyVertex.h"

class MyMesh {
public:
	MyMesh() {
		doubleSided = false;
		wireframe = false;
	}

	void createRect(float w, float h);
	void createCube(float w, float h, float d);

	void draw();
	void clear();

	std::vector<MyVertex_PosColorUv> vertices;
	std::vector<uint16_t> indices;

	bool doubleSided : 1;
	bool wireframe : 1;
};
