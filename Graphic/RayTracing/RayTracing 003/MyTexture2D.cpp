#include "precompiledHeader.h"
#include "MyTexture2D.h"

void MyTexture2D::create(int width, int height, const MyColor4b* pixels) {
	destroy();

	if (!my_is_pow2(width) || !my_is_pow2(height))
		throw MyError("width or height must be power of 2");

	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_width = width;
	m_height = height;
}

void MyTexture2D::destroy() {
	if (m_tex) {
		glDeleteTextures(1, &m_tex);
		m_tex = 0;
	}
}

void MyTexture2D::subImage(int x, int y, int width, int height, const MyColor4b* pixels) {
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MyTexture2D::bind() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MyTexture2D::unbind() {
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
