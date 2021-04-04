#pragma once

#include "MyVec.h"

class MyTriangle {
public:
	MyTriangle() = default;

	MyTriangle(const MyVec3f& v0_, const MyVec3f& v1_, const MyVec3f& v2_) 
		: v0(v0_)
		, v1(v1_)
		, v2(v2_)
	{}

	MyVec3f calcNormal() const {
		return (v1 - v0).cross(v2-v0).normalize();
	}

	MyVec3f calcCenter() const {
		return (v0 + v1 + v2) / 3.0f;
	}

	void draw() {
		glBegin(GL_LINE_LOOP);
			glVertex3fv(v0.data);
			glVertex3fv(v1.data);
			glVertex3fv(v2.data);
		glEnd();

		glBegin(GL_LINES);
			auto c = calcCenter();
			auto nl = c + calcNormal();
			glVertex3fv(c.data);
			glVertex3fv(nl.data);
		glEnd();
	}

	MyVec3f v0;
	MyVec3f v1;
	MyVec3f v2;
};

class MyPlane {
public:
	MyPlane() = default;

	MyPlane(const MyVec3f& normal_, float distance_) 
		: normal(normal_)
		, distance(distance_)
	{}

	MyPlane(const MyVec3f& normal_, const MyVec3f& point_)
		: normal(normal_)
		, distance(normal_.dot(point_))
	{}

	MyPlane(const MyTriangle& tri) {
		auto n =(tri.v1 - tri.v0).cross(tri.v2 - tri.v0).normalize();
		distance = n.dot(tri.v0);
		normal = n;
	}

	MyVec3f normal;
	float distance;
};

class MySphere {
public:
	MySphere() = default;
	MySphere(const MyVec3f& center_, float radius_) 
		: center(center_)
		, radius(radius_)
	{}

	MyVec3f center;
	float radius;
};

class MyAABB {
public:
	MyAABB() {
		setEmpty();
	}

	void setEmpty() {
		minPt.set( 1, 1, 1);
		maxPt.set(-1,-1,-1);
	}

	void unionPoint(const MyVec3f& pt) {
		if (empty()) {
			minPt = pt;
			maxPt = pt;
		}else{
			minPt = minPt.min(pt);
			maxPt = maxPt.max(pt);
		}
	}

	bool empty() const { return minPt.x > maxPt.x; }

	bool isInside(const MyVec3f& pt) const {
		if (empty()) return false;
		if (pt.x < minPt.x || pt.x > maxPt.x) return false;
		if (pt.y < minPt.y || pt.y > maxPt.y) return false;
		if (pt.z < minPt.z || pt.z > maxPt.z) return false;
		return true;
	}

	void draw() {
		Scoped_glColor color(1,1,0, 0.5f);

		//		  0--------1
		//		 /|       /|
		//		3--------2 |
		//      | |      | |
		//		| 4------|-5
		//      |/       |/
		//      7--------6
		MyVec3f v[8] = {
			MyVec3f(minPt.x, minPt.y, minPt.z),
			MyVec3f(maxPt.x, minPt.y, minPt.z),
			MyVec3f(maxPt.x, maxPt.y, minPt.z),
			MyVec3f(minPt.x, maxPt.y, minPt.z),

			MyVec3f(minPt.x, minPt.y, maxPt.z),
			MyVec3f(maxPt.x, minPt.y, maxPt.z),
			MyVec3f(maxPt.x, maxPt.y, maxPt.z),
			MyVec3f(minPt.x, maxPt.y, maxPt.z),
		};

		int16_t indices[] = {
			//top
			0,1,
			1,2,
			2,3,
			3,0,
			//bottom
			4,5,
			5,6,
			6,7,
			7,4,
			//----
			1,5,
			2,6,
			3,7,
			0,4,
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, v[0].data);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	MyVec3f minPt;
	MyVec3f maxPt;
};