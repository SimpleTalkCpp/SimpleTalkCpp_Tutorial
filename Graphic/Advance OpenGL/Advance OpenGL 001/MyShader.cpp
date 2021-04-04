#include "precompiledHeader.h"
#include "MyShader.h"

void MyShader::destroy() {
	if (m_vertexShader) {
		glDeleteShader(m_vertexShader);
		m_vertexShader = 0;
	}

	if (m_pixelShader) {
		glDeleteShader(m_pixelShader);
		m_pixelShader = 0;
	}

	if (m_program) {
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

void MyShader::bind() {
	glUseProgram(m_program);
}

void MyShader::unbind() {
	glUseProgram(0);
}

GLint MyShader::getUniformLoc(const char* name) {
	auto loc = glGetUniformLocation(m_program, name);
	if (loc < 0) {
		printf("cannot find shader uniform variable %s\n", name);
	}
	return loc;
}

void MyShader::setUniform(const char* name, float value) {
	auto loc = getUniformLoc(name);
	glUniform1fv(loc, 1, &value);
}

void MyShader::setUniform(const char* name, const MyVec2f& value) {
	auto loc = getUniformLoc(name);
	glUniform2fv(loc, 1, value.data);
}

void MyShader::setUniform(const char* name, const MyVec3f& value) {
	auto loc = getUniformLoc(name);
	glUniform3fv(loc, 1, value.data);
}

void MyShader::setUniform(const char* name, const MyVec4f& value) {
	auto loc = getUniformLoc(name);
	glUniform4fv(loc, 1, value.data);
}

void MyShader::setUniform(const char* name, const MyMatrix4f& value) {
	auto loc = getUniformLoc(name);
	glUniformMatrix4fv(loc, 1, false, value.data);
}

void MyShader::setUniformCg(const char* name, const MyMatrix4f& value) {
	auto loc = getUniformLoc(name);
	glUniform4fv(loc, 4, value.data);
}

void MyShader::reload() {
	destroy();
	{
		std::string tmp = m_filename;
		tmp += ".glsl_vs";
		_compileShader(m_vertexShader, GL_VERTEX_SHADER, tmp.c_str());
	}
	{
		std::string tmp = m_filename;
		tmp += ".glsl_ps";
		_compileShader(m_pixelShader,  GL_FRAGMENT_SHADER, tmp.c_str());
	}

	_linkProgram();
}

void MyShader::loadFile(const char* filename) {
	m_filename = filename;
	reload();
}

void MyShader::_getShaderInfoLog(GLuint shader, std::string& outMsg) {
	outMsg.clear();
	if (!shader) return;
	
	GLsizei	bufLen = 0;
	GLsizei outLen = 0;
	
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufLen);
	outMsg.resize(bufLen + 1);
	
	glGetShaderInfoLog(shader, bufLen, &outLen, &*outMsg.begin());
	outMsg.resize(outLen);
}

void MyShader::_getProgramInfoLog(GLuint program, std::string& outMsg) {
	outMsg.clear();
	if (!program) return;
	
	GLsizei	bufLen = 0;
	GLsizei outLen = 0;
	
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLen);
	outMsg.resize(bufLen + 1);
	
	glGetProgramInfoLog(program, bufLen, &outLen, &*outMsg.begin());
	outMsg.resize(outLen);
}

void MyShader::_compileShader(GLuint& shader, GLenum type, const char* filename) {
	if (!shader) {
		shader = glCreateShader(type);
	}

	std::string sourceCode;
	my_loadfile(sourceCode, filename);

	const char* srcData = sourceCode.c_str();
	      GLint srcLen  = sourceCode.size();

	glShaderSource(shader, 1, &srcData, &srcLen);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::string info;
		_getShaderInfoLog(shader, info);

		std::string tmp = "Error compile shader filename = ";
		tmp += filename;
		tmp += "\n";
		tmp += info;
		throw MyError(tmp.c_str());
	}  
}

void MyShader::_linkProgram() {
	if (!m_program) {
		m_program = glCreateProgram();
	}
	if (m_vertexShader) {
		glAttachShader(m_program, m_vertexShader);
	}

	if (m_pixelShader) {
		glAttachShader(m_program, m_pixelShader);
	}

	glLinkProgram(m_program);
	GLint linked;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	if (!linked) {
		std::string info;
		_getProgramInfoLog(m_program, info);

		std::string tmp = "Error link shader filename = ";
		tmp += m_filename;
		tmp += "\n";
		tmp += info;
		throw MyError(tmp.c_str());
	}

	//-----
	glValidateProgram(m_program);
	GLint validated;
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &validated);
	if (!validated) {
		std::string info;
		_getProgramInfoLog(m_program, info);

		std::string tmp = "Error validate shader filename = ";
		tmp += m_filename;
		tmp += "\n";
		tmp += info;
		throw MyError(tmp.c_str());
	}
}
