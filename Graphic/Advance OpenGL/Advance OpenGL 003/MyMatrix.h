#pragma once

#include "MyVec.h"

class MyMatrix4f {
public:
	union {
		struct{ MyVec4f cx, cy, cz, cw; };
		MyVec4f col[4];
		float data[16];
	};

	MyMatrix4f() = default;
	MyMatrix4f(const MyVec4f& cx_, const MyVec4f& cy_, const MyVec4f& cz_, const MyVec4f& cw_);
	MyMatrix4f(	float xx, float xy, float xz, float xw,
				float yx, float yy, float yz, float yw,
				float zx, float zy, float zz, float zw,
				float wx, float wy, float wz, float ww);

	void set(	float xx, float xy, float xz, float xw,
				float yx, float yy, float yz, float yw,
				float zx, float zy, float zz, float zw,
				float wx, float wy, float wz, float ww);

	static MyMatrix4f kIdentity();

	void setIdentity() { *this = kIdentity(); }
	void setTranslate(float x, float y, float z);
	void setTranslate(const MyVec3f& v) { setTranslate(v.x, v.y, v.z); }

	void setTRS(const MyVec3f& translate, const MyVec3f& rotate, const MyVec3f& scale);

	void setPerspective(float fovy_rad, float aspect, float zNear, float zFar);
	void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void setLookAt(const MyVec3f& eye, const MyVec3f& aim, const MyVec3f& up);

	void setRotateX(float rad);
	void setRotateY(float rad);
	void setRotateZ(float rad);
	void setScale(const MyVec3f &v);

	void translate(float x, float y, float z);
	void rotateX(float rad);
	void rotateY(float rad);
	void rotateZ(float rad);

	void operator*=(const MyMatrix4f& r) { *this = *this * r; }

	MyMatrix4f operator*(const MyMatrix4f& r) const;
	MyMatrix4f inverse() const;

	MyVec4f mulPoint(const MyVec4f& v) const;

	MyVec3f mulPoint(const MyVec3f& v) const;
	MyVec3f mulVector(const MyVec3f& v) const;
	MyVec3f mulNormal(const MyVec3f& v) const;
};

static_assert(sizeof(MyMatrix4f) == sizeof(float) * 16, "");
