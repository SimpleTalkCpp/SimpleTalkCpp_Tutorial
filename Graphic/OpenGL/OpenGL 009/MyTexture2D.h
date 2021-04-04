#pragma once

#include "MyColor.h"

#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "zlib.lib")

class MyTexture2D {
public:

	~MyTexture2D() {
		destroy();
	}

	void create();
	void destroy();

	void loadPngFile(const char* filename, bool mulAlpha);

	void bind();
	void unbind();
private:
	GLuint m_tex = 0;
};
