#pragma once

#include "MyColor.h"
#include "MyRay.h"
#include "MyMatrix.h"

class MyRayTracer {
public:

	MyRayTracer();

	void init(int canvasWidth, int canvasHeight, const MyMatrix4f& projMatrix, const MyMatrix4f& modelview);

	MyRay3f getRay(float x, float y);

	const MyColor4b* pixels() const { return m_pixels.data(); }

	void render(const MyMesh& mesh);

	const MyVec3f& lightPos() const { return m_lightPos; }

private:
	MyColor4f onPixelShading(const MyRay3f::HitResult& hit);

	MyMatrix4f	m_projMatrix;
	MyMatrix4f	m_modelview;

	MyMatrix4f	m_invProjMatrix;
	MyMatrix4f	m_invModelview;

	int			m_canvasWidth = 0;
	int			m_canvasHeight = 0;

	MyVec3f		m_lightPos;

	std::vector<MyColor4b> m_pixels;
};