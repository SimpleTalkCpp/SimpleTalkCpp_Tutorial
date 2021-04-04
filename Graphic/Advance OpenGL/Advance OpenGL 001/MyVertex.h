#pragma once

#include "MyColor.h"
#include "MyVec.h"

class alignas(4) MyVertex_PosColorUv {
public:
	MyVec3f pos;
	MyColor4b color;
	MyVec2f uv;
	MyVec3f normal;
};