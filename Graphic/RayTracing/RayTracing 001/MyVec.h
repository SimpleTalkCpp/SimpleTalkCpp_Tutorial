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

	MyVec2f() = default;

	MyVec2f(float x_, float y_) {
		x = x_;
		y = y_;
	}

	void set(float x_, float y_) {
		x = x_;
		y = y_;
	}

	MyVec2f operator+(const MyVec2f& r) const { return MyVec2f(x + r.x, y + r.y); }
	MyVec2f operator-(const MyVec2f& r) const { return MyVec2f(x - r.x, y - r.y); }
	MyVec2f operator*(const MyVec2f& r) const { return MyVec2f(x * r.x, y * r.y); }
	MyVec2f operator/(const MyVec2f& r) const { return MyVec2f(x / r.x, y / r.y); }

	MyVec2f operator+(float s) const { return MyVec2f(x + s, y + s); }
	MyVec2f operator-(float s) const { return MyVec2f(x - s, y - s); }
	MyVec2f operator*(float s) const { return MyVec2f(x * s, y * s); }
	MyVec2f operator/(float s) const { return MyVec2f(x / s, y / s); }
};

inline
std::ostream& operator << (std::ostream& s, const MyVec2f& v) {
	s << "<" << v.x << ", " << v.y << ">";
	return s;
}

class MyVec3f {
public:
	union {
		struct{
			float x, y, z;
		};
		float data[3];
	};

	MyVec3f() = default;

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

	float magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	float length() const { return magnitude(); }

	MyVec3f normalize() const {
		float m = magnitude();
		if (!m)
			return MyVec3f(0,0,0);
		auto inv = 1 / m;
		return *this * inv;
	}

	float dot(const MyVec3f& r) const {
		return (x * r.x) + (y * r.y) + (z * r.z);
	}

	MyVec3f cross(const MyVec3f& r) const {
		return MyVec3f(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}
};

inline
std::ostream& operator << (std::ostream& s, const MyVec3f& v) {
	s << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	return s;
}

class MyVec4f {
public:
	union {
		struct{
			float x, y, z, w;
		};
		float data[4];
	};

	MyVec4f() = default;

	MyVec4f(float x_, float y_, float z_, float w_) {
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	void set(float x_, float y_, float z_, float w_) {
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	MyVec3f toVec3() const {
		if (!w)
			return MyVec3f(0,0,0);
		auto invW = 1 / w;
		return MyVec3f(x,y,z) * invW;
	}

	MyVec4f operator+(const MyVec4f& r) const { return MyVec4f(x + r.x, y + r.y, z + r.z, w + r.w); }
	MyVec4f operator-(const MyVec4f& r) const { return MyVec4f(x - r.x, y - r.y, z - r.z, w - r.w); }
	MyVec4f operator*(const MyVec4f& r) const { return MyVec4f(x * r.x, y * r.y, z * r.z, w * r.w); }
	MyVec4f operator/(const MyVec4f& r) const { return MyVec4f(x / r.x, y / r.y, z / r.z, w / r.w); }

	MyVec4f operator+(float s) const { return MyVec4f(x + s, y + s, z + s, w + s); }
	MyVec4f operator-(float s) const { return MyVec4f(x - s, y - s, z - s, w - s); }
	MyVec4f operator*(float s) const { return MyVec4f(x * s, y * s, z * s, w * s); }
	MyVec4f operator/(float s) const { return MyVec4f(x / s, y / s, z / s, w / s); }

	void operator+=(float s) { x += s; y += s; z += s; w += s; }
	void operator-=(float s) { x -= s; y -= s; z -= s; w -= s; }
	void operator*=(float s) { x *= s; y *= s; z *= s; w *= s; }
	void operator/=(float s) { x /= s; y /= s; z /= s; w /= s; }
};

inline
std::ostream& operator << (std::ostream& s, const MyVec4f& v) {
	s << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
	return s;
}