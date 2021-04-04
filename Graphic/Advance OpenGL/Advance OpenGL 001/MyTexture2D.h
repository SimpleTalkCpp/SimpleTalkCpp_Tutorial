#pragma once

#include "MyColor.h"

class MyTexture2D {
public:

	~MyTexture2D() {
		destroy();
	}

	void create(int width, int height, const MyColor4b* pixels);
	void destroy();

	void subImage(int x, int y, int width, int height, const MyColor4b* pixels);

	void bind();
	void unbind();

	bool valid() const { return m_tex != 0; }
	int width () const { return m_width; }
	int height() const { return m_height; }

private:
	GLuint m_tex = 0;
	int m_width = 0;
	int m_height = 0;
};
