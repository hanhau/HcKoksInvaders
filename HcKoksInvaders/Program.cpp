#include "include/Program.hpp"
#include <GL/glew.h>
#include <iostream>
#include <glm\gtc\type_ptr.hpp>
#include <Util.hpp>

Program::Program(Shader const& vertex,Shader const& fragment) : 
	glID(glCreateProgram())
{
	assert(vertex.getType() == Shader::ShaderType::Vertex);
	assert(fragment.getType() == Shader::ShaderType::Fragment);

	glAttachShader(glID, vertex.getID());
	glAttachShader(glID, fragment.getID());
	glLinkProgram(glID);

	GLint status;
	glGetProgramiv(glID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		GLchar infoLog[1024] = { ' ' };
		GLsizei infoLength = 0;
		glGetProgramInfoLog(glID, 1024, &infoLength, infoLog);

		std::cout << infoLog << "\n";

		glGetShaderInfoLog(vertex.getID(), 1024, &infoLength, infoLog);
		std::cout << "Vertex-Shader: " << infoLog << "\n";

		glGetShaderInfoLog(fragment.getID(), 1024, &infoLength, infoLog);
		std::cout << "Fragment-Shader: " << infoLog << "\n";

		std::cout << "\n------------\n";
	}

	util::checkGlCalls(__FUNCSIG__);
}
Program::~Program() {
	//glDeleteProgram(glID);
}

const unsigned int Program::getID() const {
	return glID;
}

void Program::bind() const {
	glUseProgram(glID);

	util::checkGlCalls(__FUNCSIG__);
}

// Uniform Operations
void Program::setUniform(const std::string name, glm::mat4x4 mat4) const {
	GLuint loc = glGetUniformLocation(glID, name.c_str());

	if (loc == -1) {
		std::cout << "Name not found in program: " << name << "\n";
		return;
	}

	bind();
	glUniformMatrix4fv(loc,1,GL_FALSE,(const GLfloat*)glm::value_ptr(mat4));

	util::checkGlCalls(__FUNCSIG__);
}
void Program::setUniform(const std::string name, sf::Vector3f vec3) const {
	GLuint loc = glGetUniformLocation(glID, name.c_str());
	if (loc == -1) {
		std::cout << "Name not found in program: " << name << "\n";
		return;
	}

	const GLfloat vals[] = {vec3.x,vec3.y,vec3.z};

	bind();
	glUniform3fv(loc, 1, vals);

	util::checkGlCalls(__FUNCSIG__);
}
void Program::setUniform(const std::string name, sf::Vector2f vec2) const {
	GLuint loc = glGetUniformLocation(glID, name.c_str());
	if (loc == -1) {
		std::cout << "Name not found in program: " << name << "\n";
		return;
	}

	const GLfloat vals[] = {vec2.x,vec2.y};

	bind();
	glUniform2fv(loc, 1, vals);

	util::checkGlCalls(__FUNCSIG__);
}

void Program::setUniform(const std::string name, float f) const {
	GLuint loc = glGetUniformLocation(glID, name.c_str());
	if (loc == -1) {
		std::cout << "Name not found in program: " << name << "\n";
		return;
	}

	bind();
	glUniform1f(loc, f);

	util::checkGlCalls(__FUNCSIG__);
}
void Program::setUniform(const std::string name, int i) const {
	GLuint loc = glGetUniformLocation(glID, name.c_str());
	if (loc == -1) {
		std::cout << "Name not found in program: " << name << "\n";
		return;
	}

	bind();
	glUniform1i(loc, i);

	util::checkGlCalls(__FUNCSIG__);
}