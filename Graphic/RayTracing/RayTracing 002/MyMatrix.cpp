#include "precompiledHeader.h"
#include "MyMatrix.h"

MyMatrix4f MyMatrix4f::inverse() const {
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = cx.x;
	r0[1] = cx.y;
	r0[2] = cx.z;
	r0[3] = cx.w;
	r0[4] = 1.0;
	r0[5] = r0[6] = r0[7] = 0.0;

	r1[0] = cy.x;
	r1[1] = cy.y;
	r1[2] = cy.z;
	r1[3] = cy.w;
	r1[5] = 1.0;
	r1[4] = r1[6] = r1[7] = 0.0;

	r2[0] = cz.x;
	r2[1] = cz.y;
	r2[2] = cz.z;
	r2[3] = cz.w;
	r2[6] = 1.0;
	r2[4] = r2[5] = r2[7] = 0.0;

	r3[0] = cw.x;
	r3[1] = cw.y;
	r3[2] = cw.z;
	r3[3] = cw.w;
	r3[7] = 1.0;
	r3[4] = r3[5] = r3[6] = 0.0;

	// choose pivot - or die
	if (std::abs(r3[0]) > std::abs(r2[0])) std::swap(r3, r2);
	if (std::abs(r2[0]) > std::abs(r1[0])) std::swap(r2, r1);
	if (std::abs(r1[0]) > std::abs(r0[0])) std::swap(r1, r0);
	if (0.0 == r0[0]) return kIdentity();

	// eliminate first variable
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];

	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4]; if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5]; if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6]; if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7]; if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	// choose pivot - or die
	if (std::abs(r3[1]) > std::abs(r2[1])) std::swap(r3, r2);
	if (std::abs(r2[1]) > std::abs(r1[1])) std::swap(r2, r1);
	if (0.0 == r1[1]) {
		assert(false);
		return kIdentity();
	}

	// eliminate second variable
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];

	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	// choose pivot - or die
	if (std::abs(r3[2]) > std::abs(r2[2])) std::swap(r3, r2);
	if (0.0 == r2[2]) {
		assert(false);
		return kIdentity();
	}

	// eliminate third variable
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3];
	r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5];
	r3[6] -= m3 * r2[6],
		r3[7] -= m3 * r2[7];

	// last check
	if (0.0 == r3[3]) {
		assert(false);
		return kIdentity();
	}

	s = 1.0f / r3[3];    // now back substitute row 3
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];          // now back substitute row 2
	s = 1.0f / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1;
	r1[5] -= r3[5] * m1;
	r1[6] -= r3[6] * m1;
	r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0;
	r0[5] -= r3[5] * m0;
	r0[6] -= r3[6] * m0;
	r0[7] -= r3[7] * m0;

	m1 = r1[2];                 // now back substitute row 1
	s = 1.0f / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1);
	r1[5] = s * (r1[5] - r2[5] * m1);
	r1[6] = s * (r1[6] - r2[6] * m1);
	r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0;
	r0[5] -= r2[5] * m0;
	r0[6] -= r2[6] * m0;
	r0[7] -= r2[7] * m0;

	m0 = r0[1];                 // now back substitute row 0
	s = 1.0f / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0);
	r0[5] = s * (r0[5] - r1[5] * m0);
	r0[6] = s * (r0[6] - r1[6] * m0);
	r0[7] = s * (r0[7] - r1[7] * m0);

	return MyMatrix4f(r0[4], r0[5], r0[6], r0[7],
					  r1[4], r1[5], r1[6], r1[7],
					  r2[4], r2[5], r2[6], r2[7],
					  r3[4], r3[5], r3[6], r3[7]);
}

MyMatrix4f MyMatrix4f::operator*(const MyMatrix4f& r) const {
	MyMatrix4f o;

	float e0, e1, e2, e3;
	e0 = cx.x, e1 = cy.x, e2 = cz.x, e3 = cw.x;
	o.cx.x = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.x = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.x = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.x = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	e0 = cx.y, e1 = cy.y, e2 = cz.y, e3 = cw.y;
	o.cx.y = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.y = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.y = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.y = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	e0 = cx.z, e1 = cy.z, e2 = cz.z, e3 = cw.z;
	o.cx.z = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.z = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.z = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.z = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	e0 = cx.w, e1 = cy.w, e2 = cz.w, e3 = cw.w;
	o.cx.w = e0*r.cx.x + e1*r.cx.y + e2*r.cx.z + e3*r.cx.w;
	o.cy.w = e0*r.cy.x + e1*r.cy.y + e2*r.cy.z + e3*r.cy.w;
	o.cz.w = e0*r.cz.x + e1*r.cz.y + e2*r.cz.z + e3*r.cz.w;
	o.cw.w = e0*r.cw.x + e1*r.cw.y + e2*r.cw.z + e3*r.cw.w;

	return o;	
}

MyVec4f MyMatrix4f::mulPoint(const MyVec4f& v) const {
	// mul 4x4
	return MyVec4f(	v.x * cx.x + v.y * cy.x + v.z * cz.x + v.w * cw.x,
					v.x * cx.y + v.y * cy.y + v.z * cz.y + v.w * cw.y,
					v.x * cx.z + v.y * cy.z + v.z * cz.z + v.w * cw.z,
					v.x * cx.w + v.y * cy.w + v.z * cz.w + v.w * cw.w);
}

MyVec3f MyMatrix4f::mulPoint(const MyVec3f& v) const {
	// mul 4x3
	return MyVec3f(	v.x * cx.x + v.y * cy.x + v.z * cz.x + cw.x,
					v.x * cx.y + v.y * cy.y + v.z * cz.y + cw.y,
					v.x * cx.z + v.y * cy.z + v.z * cz.z + cw.z);
}

MyVec3f MyMatrix4f::mulVector(const MyVec3f& v) const {
	// mul 3x3
	return MyVec3f(	v.x * cx.x + v.y * cy.x + v.z * cz.x,
					v.x * cx.y + v.y * cy.y + v.z * cz.y,
					v.x * cx.z + v.y * cy.z + v.z * cz.z);
}

MyVec3f MyMatrix4f::mulNormal(const MyVec3f& v) const {
	// mul inverse transpose

	auto invM = inverse();
	return MyVec3f(	v.x * cx.x + v.y * cx.y + v.z * cx.z,
					v.x * cy.x + v.y * cy.y + v.z * cy.z,
					v.x * cz.x + v.y * cz.y + v.z * cz.z);
}

MyRay3f MyMatrix4f::mul(const MyRay3f& ray) {
	return MyRay3f(	mulPoint(ray.origin), 
					mulVector(ray.direction));
}
