#pragma once

#include "MyVertex.h"
#include "MyMatrix.h"

class MyMesh {
public:
	using IndexType = uint16_t;

	MyMesh() {
		doubleSided = false;
		wireframe = false;
	}

	void createRect(float w, float h);
	void createCube(float w, float h, float d);

	void loadObjFile(const char* filename);

	void draw();

	void clear();

	void setColor(const MyColor4b& c);

	std::vector<MyVertex_PosColorUv> vertices;
	std::vector<IndexType> indices;

	bool doubleSided : 1;
	bool wireframe : 1;
};
