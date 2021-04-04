#pragma once

#include "MyVec.h"
#include "MyGeometry.h"

class MyMatrix4f;
class MyMesh;

class MyRay3f {
public:
	MyRay3f() = default;

	MyRay3f(const MyVec3f& origin_, const MyVec3f& direction_) 
		: origin(origin_)
		, direction(direction_)
	{}

	class HitResult {
	public:
		HitResult() {
			reset();
		}

		void reset() {
			distance = std::numeric_limits<float>::infinity();
			hasResult = false;
		}

		MyVec3f point;
		MyVec3f normal;
		float   distance;
		bool	hasResult;
	};

	bool raycast(HitResult& outResult, const MyPlane&    plane,  float maxDistance);
	bool raycast(HitResult& outResult, const MySphere&   sphere, float maxDistance);
	bool raycast(HitResult& outResult, const MyTriangle& tri,    float maxDistance);
	bool raycast(HitResult& outResult, const MyMesh&     mesh,   float maxDistance);

	bool raycast(const MyAABB& aabb);

	static MyRay3f unprojectFromInverseMatrix(const MyMatrix4f& invProj, const MyMatrix4f& invModelview, const MyVec2f& pointOnScreen, const MyVec2f& screenSize);

	void draw() {
		Scoped_glColor color(1,0,1,1);
		glBegin(GL_POINTS);
			glVertex3fv(origin.data);
		glEnd();
		glBegin(GL_LINES);
			glVertex3fv(origin.data);
			auto v = origin + direction;
			glVertex3fv(v.data);
		glEnd();
	}

	MyVec3f origin;
	MyVec3f direction;
};