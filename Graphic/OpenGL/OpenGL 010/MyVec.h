#pragma once

#include "my_common.h"

class MyVec2f {
public:
	union {
		struct{
			float x, y;
		};
		float data[2];
	};

	MyVec2f() {}

	MyVec2f(float x_, float y_) {
		x = x_;
		y = y_;
	}

	void set(float x_, float y_) {
		x = x_;
		y = y_;
	}

	MyVec2f operator*(const MyVec2f& r) const { return MyVec2f(x * r.x, y * r.y); }

	MyVec2f operator+(float s) const { return MyVec2f(x + s, y + s); }
	MyVec2f operator-(float s) const { return MyVec2f(x - s, y - s); }
	MyVec2f operator*(float s) const { return MyVec2f(x * s, y * s); }
	MyVec2f operator/(float s) const { return MyVec2f(x / s, y / s); }
};

class MyVec3f {
public:
	union {
		struct{
			float x, y, z;
		};
		float data[3];
	};

	MyVec3f() {}

	MyVec3f(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	void set(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	MyVec3f operator+(const MyVec3f& r) const { return MyVec3f(x + r.x, y + r.y, z + r.z); }
	MyVec3f operator-(const MyVec3f& r) const { return MyVec3f(x - r.x, y - r.y, z - r.z); }
	MyVec3f operator*(const MyVec3f& r) const { return MyVec3f(x * r.x, y * r.y, z * r.z); }
	MyVec3f operator/(const MyVec3f& r) const { return MyVec3f(x / r.x, y / r.y, z / r.z); }

	MyVec3f operator+(float s) const { return MyVec3f(x + s, y + s, z + s); }
	MyVec3f operator-(float s) const { return MyVec3f(x - s, y - s, z - s); }
	MyVec3f operator*(float s) const { return MyVec3f(x * s, y * s, z * s); }
	MyVec3f operator/(float s) const { return MyVec3f(x / s, y / s, z / s); }

	void operator+=(float s) { x += s; y += s; z += s; }
	void operator-=(float s) { x -= s; y -= s; z -= s; }
	void operator*=(float s) { x *= s; y *= s; z *= s; }
	void operator/=(float s) { x /= s; y /= s; z /= s; }
};