#include "include/Shader.hpp"
#include <fstream>
#include <GL/glew.h>

Shader::Shader() : glID(0), m_shaderType(ShaderType::Unitialized) {}
Shader::~Shader() {
	//glDeleteShader(glID);
}

bool Shader::loadFromMemory(uint8_t* buffer, size_t bufferLength,Shader::ShaderType shaderType) {
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

	GLchar const* files[] = { (GLchar*)buffer };
	GLint lengths[] = { bufferLength };
	glShaderSource(glID, 1, files, lengths);
	glCompileShader(glID);

	return true;
}
bool Shader::loadFromString(const std::string str, Shader::ShaderType shaderType) {
	return loadFromMemory((uint8_t*)str.data(), str.length(), shaderType);
}
bool Shader::loadFromPreloadData(const PreloadData& pData, ShaderType shaderType) {
	return loadFromMemory(pData.m_data.get(), pData.m_dataLength, shaderType);
}

const Shader::ShaderType& Shader::getType() const {
	return m_shaderType;
}

unsigned int Shader::getID() const {
	return glID;
}