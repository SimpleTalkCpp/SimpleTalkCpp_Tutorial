#pragma once

#include "MyVertex.h"
#include "MyMatrix.h"

class MyVertexBuffer {
public:
	~MyVertexBuffer() { destroy(); }

	void create(const MyVertex_PosColorUv* p, size_t n);
	void destroy();

	void bind	() const { glBindBuffer(GL_ARRAY_BUFFER, m_p); }
	void unbind	() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

private:
	GLuint m_p = 0;
};

class MyIndexBuffer{
public:
	~MyIndexBuffer() { destroy(); }

	void create(const uint16_t* p, size_t n);
	void destroy();

	void bind	() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_p); }
	void unbind	() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

private:
	GLuint m_p = 0;
};

class MyMesh {
public:
	using IndexType = uint16_t;

	MyMesh() {
		doubleSided = false;
		wireframe = false;
	}

	void createRect(float w, float h);
	void createCube(float w, float h, float d);
	void createGrid(int w, int h, float cellSize, const MyColor4b& color);
	void createOrigin();

	void loadObjFile(const char* filename);

	void draw();
	void clear();

	void setColor(const MyColor4b& c);

	void updateVBO();

	std::vector<MyVertex_PosColorUv> vertices;
	std::vector<IndexType> indices;

	bool doubleSided : 1;
	bool wireframe : 1;

	MyVertexBuffer vertexBuffer;
	MyIndexBuffer  indexBuffer;

	GLenum drawMode = GL_TRIANGLES;
};
