#pragma once

#include "MyVertex.h"
#include "MyGeometry.h"

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
	void updateAABB();

	void setColor(const MyColor4b& c);

	std::vector<MyVertex_PosColorUv> vertices;
	std::vector<IndexType> indices;

	MyAABB aabb;

	bool doubleSided : 1;
	bool wireframe : 1;
};
