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
	MyMatrix4f(const MyVec4f& cx_, const MyVec4f& cy_, const MyVec4f& cz_, const MyVec4f& cw_) {
		cx = cx_;
		cy = cy_;
		cz = cz_;
		cw = cw_;
	}
	MyMatrix4f(	float xx, float xy, float xz, float xw,
				float yx, float yy, float yz, float yw,
				float zx, float zy, float zz, float zw,
				float wx, float wy, float wz, float ww) 
	{
		cx.set(xx, xy, xz, xw);
		cy.set(yx, yy, yz, yw);
		cz.set(zx, zy, zz, zw);
		cw.set(wx, wy, wz, ww);
	}


	static MyMatrix4f kIdentity() {
		return MyMatrix4f(1,0,0,0,
						  0,1,0,0,
						  0,0,1,0,
						  0,0,0,1);
	}

	MyMatrix4f operator*(const MyMatrix4f& r) const;
	MyMatrix4f inverse() const;

	MyVec4f mulPoint(const MyVec4f& v) const;

	MyVec3f mulPoint(const MyVec3f& v) const;
	MyVec3f mulVector(const MyVec3f& v) const;
	MyVec3f mulNormal(const MyVec3f& v) const;
};

static_assert(sizeof(MyMatrix4f) == sizeof(float) * 16, "");
