#pragma once

#include "MyVec.h"

class MyMatrix4f;

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
			distance = std::numeric_limits<float>::max();
			hasResult = false;
		}

		MyVec3f point;
		MyVec3f normal;
		float   distance;
		bool	hasResult;
	};

	static MyRay3f unprojectFromInverseMatrix(const MyMatrix4f& invProj, const MyMatrix4f& invModelview, const MyVec2f& pointOnScreen, const MyVec2f& screenSize);

	MyVec3f origin;
	MyVec3f direction;
};