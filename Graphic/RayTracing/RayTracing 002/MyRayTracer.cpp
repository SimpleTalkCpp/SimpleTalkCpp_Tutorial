#include "precompiledHeader.h"
#include "MyRayTracer.h"

void MyRayTracer::init(const MyVec2f& screenSize, const MyMatrix4f& projMatrix, const MyMatrix4f& modelview) {
	m_screenSize = screenSize;

	m_projMatrix = projMatrix;
	m_modelview = modelview;

	m_invProjMatrix = projMatrix.inverse();
	m_invModelview  = modelview.inverse();
}

MyRay3f MyRayTracer::getRay(float x, float y) {
	return MyRay3f::unprojectFromInverseMatrix(m_invProjMatrix, m_invModelview, MyVec2f(x,y), m_screenSize);
}
