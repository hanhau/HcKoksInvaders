#pragma once
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

class Program {
	const unsigned int glID;
public:
	Program(Shader const& vertex, Shader const& fragment);
	~Program();

	const unsigned int getID() const;
	void bind() const;

	void setUniform(const std::string name, glm::mat4x4 mat4) const;
	void setUniform(const std::string name, sf::Vector3f vec3) const;
	void setUniform(const std::string name, sf::Vector2f vec2) const;
	void setUniform(const std::string name, float f) const;
	void setUniform(const std::string name, int i) const;
};