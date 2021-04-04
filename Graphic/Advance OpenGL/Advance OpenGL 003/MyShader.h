#pragma once

#include "MyMatrix.h"
#include "MyMesh.h"
#include "MyTexture2D.h"

class MyVertexArray : public MyNonCopyable {
public:
	~MyVertexArray() { destroy(); }

	void create();
	void destroy();

	void bind	() const { glBindVertexArray(m_p); }
	void unbind	() const { glBindVertexArray(0); }

private:
	GLuint m_p = 0;
};

class MySampler : public MyNonCopyable {
public:
	~MySampler() { destroy(); }

	void create();
	void destroy();

	void bind	(int texUnit) const { glBindSampler(texUnit, m_p); }
	void unbind	(int texUnit) const { glBindSampler(texUnit, 0); }

private:
	GLuint  m_p = 0;
};

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

	void setUniform(const char* name, const MyTexture2D& tex);

	void reload();

	GLint getUniformLoc(const char* name);

	void draw(const MyMesh& mesh);
	void dumpActiveAttrib();

	static const int kMaxSamplers = 16;

private:

	void _getShaderInfoLog	(GLuint shader, std::string& outMsg);
	void _getProgramInfoLog	(GLuint shader, std::string& outMsg);

	void _compileShader(GLuint& shader, GLenum type, const char* filename);
	void _linkProgram();

	GLuint m_program = 0;
	GLuint m_vertexShader = 0;
	GLuint m_pixelShader = 0;

	MyVertexArray m_vertexArray;

	bool m_reportError = true;

	int m_boundTexCount = 0;

	struct TextureUnit {
		MySampler sampler;
		const MyTexture2D*	tex = nullptr;
	};
	TextureUnit m_texUnits[kMaxSamplers];

	std::string m_filename;
};