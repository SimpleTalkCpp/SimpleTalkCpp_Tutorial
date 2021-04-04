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

MyMatrix4f::MyMatrix4f(	float xx, float xy, float xz, float xw, 
						float yx, float yy, float yz, float yw,
						float zx, float zy, float zz, float zw,
						float wx, float wy, float wz, float ww)
{
	cx.set(xx, xy, xz, xw);
	cy.set(yx, yy, yz, yw);
	cz.set(zx, zy, zz, zw);
	cw.set(wx, wy, wz, ww);
}

MyMatrix4f::MyMatrix4f(const MyVec4f& cx_, const MyVec4f& cy_, const MyVec4f& cz_, const MyVec4f& cw_) {
	cx = cx_;
	cy = cy_;
	cz = cz_;
	cw = cw_;
}

void MyMatrix4f::set(	float xx, float xy, float xz, float xw, 
						float yx, float yy, float yz, float yw, 
						float zx, float zy, float zz, float zw,
						float wx, float wy, float wz, float ww)
{
	cx.set(xx, xy, xz, xw);
	cy.set(yx, yy, yz, yw);
	cz.set(zx, zy, zz, zw);
	cw.set(wx, wy, wz, ww);
}

MyMatrix4f MyMatrix4f::kIdentity() {
	return MyMatrix4f(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

void MyMatrix4f::setTranslate(float x, float y, float z) {
	set(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1);
}

void MyMatrix4f::setTRS(const MyVec3f& translate, const MyVec3f& rotate, const MyVec3f& scale) {
	MyVec3f s, c;
	my_sincos(rotate.x, s.x, c.x);
	my_sincos(rotate.y, s.y, c.y);
	my_sincos(rotate.z, s.z, c.z);

	set(scale.x * (c.y*c.z),				scale.x * (c.y*s.z),               scale.x * (-s.y),    0,
		scale.y * (s.x*s.y*c.z - c.x*s.z),	scale.y * (c.x*c.z + s.x*s.y*s.z), scale.y * (s.x*c.y), 0,
		scale.z * (s.x*s.z + c.x*s.y*c.z),	scale.z * (c.x*s.y*s.z - s.x*c.z), scale.z * (c.x*c.y), 0,
		translate.x, translate.y, translate.z, 1);
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

void MyMatrix4f::setPerspective(float fovy_rad, float aspect, float zNear, float zFar ) {
	float s, c, deltaZ;
	float fov = fovy_rad / 2;
	
	deltaZ = zFar - zNear;
	s = sin( fov );
	
	if ((deltaZ == 0) || (s == 0) || (aspect == 0)) {
		setIdentity();
		return;
	}
	c = cos(fov) / s;
	
	cx.set( c / aspect, 0,   0,                          0  );
	cy.set( 0,          c,   0,                          0  );
	cz.set( 0,          0,   -(zFar + zNear) / deltaZ,   -1 );
	cw.set( 0,          0,   -2 * zNear * zFar / deltaZ, 0  );
}

void MyMatrix4f::setOrtho(float left, float right, float bottom, float top, float zNear, float zFar) {
	float w = right - left;
	float h = top - bottom;
	float d = zFar - zNear;

	if( w == 0 || h == 0 || d == 0 ) {
		setIdentity();
	}else{
		set( 2/w,   0,    0,  0,
			   0, 2/h,    0,  0,
			   0,   0, -2/d,  0,
		  -(right+left) / w, -(top+bottom) / h, -(zFar+zNear ) / d, 1 );
	}
}

void MyMatrix4f::setLookAt ( const MyVec3f& eye, const MyVec3f& aim, const MyVec3f& up ) {
	MyVec3f f = ( aim - eye ).normalize();
	MyVec3f s = f.cross(up).normalize();
	MyVec3f u = s.cross(f);

	MyMatrix4f m;
	m.set(  s.x,  u.x, -f.x, 0.0,
			s.y,  u.y, -f.y, 0.0,
			s.z,  u.z, -f.z, 0.0,
			0,    0,   0,    1);

	MyMatrix4f t;
	t.setTranslate(-eye);

	*this = m * t;
}

void MyMatrix4f::setRotateX(float rad) {
	float s, c;
	my_sincos(rad, s, c);
	cx.set( 1, 0, 0, 0 );
	cy.set( 0, c, s, 0 );
	cz.set( 0,-s, c, 0 );
	cw.set( 0, 0, 0, 1 );
}

void MyMatrix4f::setRotateY(float rad) {
	float s, c;
	my_sincos(rad, s, c);
	cx.set( c, 0,-s, 0 );
	cy.set( 0, 1, 0, 0 );
	cz.set( s, 0, c, 0 );
	cw.set( 0, 0, 0, 1 );
}

void MyMatrix4f::setRotateZ(float rad) {
	float s, c;
	my_sincos(rad, s, c);
	cx.set( c, s, 0, 0 );
	cy.set(-s, c, 0, 0 );
	cz.set( 0, 0, 1, 0 );
	cw.set( 0, 0, 0, 1 );
}

void MyMatrix4f::setScale(const MyVec3f &v) {
	cx.set( v.x,   0,   0, 0 );
	cy.set(   0, v.y,   0, 0 );
	cz.set(   0,   0, v.z, 0 );
	cw.set(   0,   0,   0, 1 );
}

void MyMatrix4f::translate(float x, float y, float z) {
	MyMatrix4f m;
	m.setTranslate(x, y, z);
	operator*=(m);
}

void MyMatrix4f::rotateX(float rad) {
	MyMatrix4f m;
	m.setRotateX(rad);
	operator*=(m);
}

void MyMatrix4f::rotateY(float rad) {
	MyMatrix4f m;
	m.setRotateY(rad);
	operator*=(m);
}

void MyMatrix4f::rotateZ(float rad) {
	MyMatrix4f m;
	m.setRotateZ(rad);
	operator*=(m);
}
