#pragma once

#include "MyMatrix.h"

class MyShader : public MyNonCopyable {
public:

	~MyShader() { destroy(); }

	void loadFile(const char* filename);
	void destroy();

	void bind();
	void unbind();

	void setUniform(const char* name, float value);
	void setUniform(const char* name, const MyVec2f& value);
	void setUniform(const char* name, const MyVec3f& value);
	void setUniform(const char* name, const MyVec4f& value);
	void setUniform(const char* name, const MyMatrix4f& value);
	void setUniformCg(const char* name, const MyMatrix4f& value);

	void reload();

	GLint getUniformLoc(const char* name);

private:

	void _getShaderInfoLog	(GLuint shader, std::string& outMsg);
	void _getProgramInfoLog	(GLuint shader, std::string& outMsg);

	void _compileShader(GLuint& shader, GLenum type, const char* filename);
	void _linkProgram();

	GLuint m_program = 0;
	GLuint m_vertexShader = 0;
	GLuint m_pixelShader = 0;

	std::string m_filename;
};