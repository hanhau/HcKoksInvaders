#include "include/Shader.hpp"
#include <fstream>
#include <GL/glew.h>

Shader::Shader() : glID(0), m_shaderType(ShaderType::Unitialized) {}
Shader::~Shader() {
	//glDeleteShader(glID);
}

bool Shader::loadFromFile(const std::string path,Shader::ShaderType shaderType) {
	m_shaderType = shaderType;
	
	std::fstream file(path, std::ios::in | std::ios::ate);
	if (!file.is_open())
		return false;

	size_t length(file.tellg());
	std::string content(length, ' ');

	file.seekg(0, std::ios_base::beg);
	file.read(&content[0], length);
	file.close();

	switch (shaderType) {
	case ShaderType::Fragment:
		glID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case ShaderType::Vertex:
		glID = glCreateShader(GL_VERTEX_SHADER);
		break;
	default: break;
	}

	GLchar const* files[] = { &content[0] };
	GLint lengths[] = { length };
	glShaderSource(glID, 1, files, lengths);
	glCompileShader(glID);

	return true;
}
bool Shader::loadFromString(const std::string str, Shader::ShaderType shaderType) {
	m_shaderType = shaderType;
	
	switch (shaderType) {
	case ShaderType::Fragment:
		glID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case ShaderType::Vertex:
		glID = glCreateShader(GL_VERTEX_SHADER);
		break;
	default: break;
	}

	GLchar const* files[] = { &str[0] };
	GLint lengths[] = { str.length() };
	glShaderSource(glID, 1, files, lengths);
	glCompileShader(glID);

	return true;
}

const Shader::ShaderType& Shader::getType() const {
	return m_shaderType;
}

unsigned int Shader::getID() const {
	return glID;
}