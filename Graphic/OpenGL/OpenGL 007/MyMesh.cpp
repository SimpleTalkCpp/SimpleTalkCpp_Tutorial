#include "MyMesh.h"

void MyMesh::createRect(float w, float h) {
	vertices.resize(4);

	float x = w / 2;
	float y = h / 2;

	auto* v = vertices.data();

	v->pos.set(-x,-y, 0); v->color.set(255, 255, 255, 255); v->uv.set(0,1); v++;
	v->pos.set( x,-y, 0); v->color.set(255, 255, 255, 255); v->uv.set(1,1); v++;
	v->pos.set( x, y, 0); v->color.set(255, 255, 255, 255); v->uv.set(1,0); v++;
	v->pos.set(-x, y, 0); v->color.set(255, 255, 255, 255); v->uv.set(0,0); v++;

	indices.assign({
		0,1,2, 
		0,2,3,
	});
}

void MyMesh::createCube(float w, float h, float d) {
	vertices.resize(8);

	float x = w / 2;
	float y = h / 2;
	float z = d / 2;

	auto* v = vertices.data();

	v->pos.set(-x,-y, z); v->color.set(255, 255, 255, 255); v->uv.set(0,1); v++;
	v->pos.set( x,-y, z); v->color.set(255, 255, 255, 255); v->uv.set(1,1); v++;
	v->pos.set( x, y, z); v->color.set(255, 255, 255, 255); v->uv.set(1,0); v++;
	v->pos.set(-x, y, z); v->color.set(255, 255, 255, 255); v->uv.set(0,0); v++;
	v->pos.set(-x,-y,-z); v->color.set(255, 255, 255, 255); v->uv.set(0,1); v++;
	v->pos.set( x,-y,-z); v->color.set(255, 255, 255, 255); v->uv.set(1,1); v++;
	v->pos.set( x, y,-z); v->color.set(255, 255, 255, 255); v->uv.set(1,0); v++;
	v->pos.set(-x, y,-z); v->color.set(255, 255, 255, 255); v->uv.set(0,0); v++;

	indices.assign({
		0,1,2, 
		0,2,3,
		2,1,5,
		2,5,6,
		3,2,6,
		3,6,7,
		0,3,7,
		0,7,4,
		1,0,4,
		1,4,5,
		4,7,6,
		4,6,5,
	});
}

void MyMesh::draw() {
	if (indices.size() <= 0)
		return;

	auto stride = sizeof(vertices[0]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride, vertices[0].pos.data);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, stride, vertices[0].color.data);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, stride, vertices[0].uv.data);

	//---- set render state
	if (!doubleSided)
		glEnable(GL_CULL_FACE);

	if (wireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);


	//-- draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data());

	//---- restore render state
	if (wireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

	if (!doubleSided)
		glDisable(GL_CULL_FACE);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MyMesh::clear() {
	vertices.clear();
	indices.clear();
}

void MyMesh::setColor(const MyColor4b& c) {
	for (auto& v : vertices) {
		v.color = c;
	}
}
