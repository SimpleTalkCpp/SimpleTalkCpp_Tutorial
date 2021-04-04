#include "precompiledHeader.h"
#include "MyRay.h"
#include "MyMatrix.h"

MyRay3f MyRay3f::unprojectFromInverseMatrix(const MyMatrix4f& invProj, const MyMatrix4f& invModelview, const MyVec2f& pointOnScreen, const MyVec2f& screenSize) {

	auto s = pointOnScreen / screenSize * 2 - 1;
	s.y = -s.y;

	auto a = MyVec4f(s.x, s.y, 0, 1);
	auto b = MyVec4f(s.x, s.y, 1, 1);

	a = invProj.mulPoint(a);
	b = invProj.mulPoint(b);

	a = invModelview.mulPoint(a);
	b = invModelview.mulPoint(b);

	MyRay3f o;
	o.origin = a.toVec3();
	o.direction = (b.toVec3() - o.origin).normalize();
	return o;
}

