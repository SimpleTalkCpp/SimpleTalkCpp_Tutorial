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
		if (m_reportError) {
			printf("cannot find uniform variable \"%s\"\nfile=%s\n\n", name, m_filename.c_str());
			m_reportError = false;
		}
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

void MyShader::dumpActiveAttrib() {
	GLint activeCount = 0;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES,           &activeCount);

	printf("--- %d active attrib ---\n", activeCount);

	for (GLint i = 0; i < activeCount; i++) {
		GLsizei outLen = 0;
		GLint   dataSize = 0;
		GLenum  dataType = 0;
		char buf[1024 + 1];
		glGetActiveAttrib(m_program, i, 1024, &outLen, &dataSize, &dataType, buf);
		buf[1024] = 0;
		printf("vertex attrib[%d] = %s\n", i, buf);
	}
	printf("\n");
}

void MyVertexArray::create() {
	if (!m_p) {
		glGenVertexArrays(1, &m_p);
	}
}

void MyVertexArray::destroy() {
	if (m_p) {
		glDeleteVertexArrays(1, &m_p);
		m_p = 0;
	}
}

class Scoped_MyShader_VertexAttrib {
public:
	Scoped_MyShader_VertexAttrib(GLuint program, const char* name, GLint numComponets, GLenum type, bool normalized, GLsizei stride, intptr_t vertexBufferOffset) {
		m_loc = glGetAttribLocation(program, name);
		if (m_loc < 0)
			return;
		glVertexAttribPointer(m_loc, numComponets, type, normalized, stride, (void*)vertexBufferOffset);
		glEnableVertexAttribArray(m_loc);
	}

	~Scoped_MyShader_VertexAttrib() {
		if (m_loc < 0)
			return;
		glDisableVertexAttribArray(m_loc);
	}

private:
	GLint m_loc = -1;
};

void MyShader::draw(const MyMesh& mesh) {
	if (!m_program)
		return;

	if (!mesh.vertices.size())
		return;

	m_vertexArray.create();
	m_vertexArray.bind();

	mesh.vertexBuffer.bind();
	mesh.indexBuffer.bind();

	GLsizei stride = sizeof(mesh.vertices[0]);
	Scoped_MyShader_VertexAttrib v_pos	 (m_program, "v_pos",    3, GL_FLOAT,         true, stride, my_offsetof(&MyVertex_PosColorUv::pos));
	Scoped_MyShader_VertexAttrib v_color (m_program, "v_color",  4, GL_UNSIGNED_BYTE, true, stride, my_offsetof(&MyVertex_PosColorUv::color));
	Scoped_MyShader_VertexAttrib v_uv	 (m_program, "v_uv",     2, GL_FLOAT,         true, stride, my_offsetof(&MyVertex_PosColorUv::uv));
	Scoped_MyShader_VertexAttrib v_normal(m_program, "v_normal", 3, GL_FLOAT,         true, stride, my_offsetof(&MyVertex_PosColorUv::normal));

	//---- set render state
	if (!mesh.doubleSided)
		glEnable(GL_CULL_FACE);

	if (mesh.wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (mesh.indices.size()) {
		void* indexBufferOffset = 0;
		glDrawElements(mesh.drawMode, mesh.indices.size(), GL_UNSIGNED_SHORT, indexBufferOffset);
	}else{
		glDrawArrays(mesh.drawMode, 0, mesh.vertices.size());
	}

	//---- restore render state
	if (mesh.wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (!mesh.doubleSided)
		glDisable(GL_CULL_FACE);

	//----------
	m_vertexArray.unbind();
	mesh.vertexBuffer.unbind();
	mesh.indexBuffer.unbind();
}
