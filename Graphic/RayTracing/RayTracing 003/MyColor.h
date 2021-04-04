#pragma once

#include "my_common.h"

class MyColor4b {
public:
	union {
		struct {
			uint8_t r,g,b,a;
		};
		uint8_t data[4];
	};

	MyColor4b() {}

	MyColor4b(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	void set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}
};

class MyColor4f {
public:
	union {
		struct {
			float r,g,b,a;
		};
		float data[4];
	};

	MyColor4f() {}

	MyColor4f(float r_, float g_, float b_, float a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	void set(float r_, float g_, float b_, float a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	MyColor4b toColor4b() const {
		return MyColor4b(
			static_cast<uint8_t>(my_clamp(r * 255, 0, 255.0f)),
			static_cast<uint8_t>(my_clamp(g * 255, 0, 255.0f)),
			static_cast<uint8_t>(my_clamp(b * 255, 0, 255.0f)),
			static_cast<uint8_t>(my_clamp(a * 255, 0, 255.0f))
		);
	}
};