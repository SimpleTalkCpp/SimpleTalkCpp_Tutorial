#pragma once

#include "MyRay.h"
#include "MyMatrix.h"

class MyRayTracer {
public:

	void init(const MyVec2f& screenSize, const MyMatrix4f& projMatrix, const MyMatrix4f& modelview);

	MyRay3f getRay(float x, float y);

	MyMatrix4f	m_projMatrix;
	MyMatrix4f	m_modelview;

	MyMatrix4f	m_invProjMatrix;
	MyMatrix4f	m_invModelview;

	MyVec2f		m_screenSize;
};