#pragma once
#include <string>
#include <functional>
#include <array>
#include <glm/vec2.hpp>
#include "Program.hpp"

class Button
{
private:
	std::string m_title;
	glm::vec2 m_position;

	unsigned int gl_vao;
	unsigned int gl_vbo;
public:
	Button(std::string title, glm::vec2 pos, int fontHeight);

	void draw(const Program& program) const;
	bool containsPoint(const glm::vec2 point) const;

	std::function<void(const Button&)> onHover;
	bool onHoverActive;
	std::function<void(const Button&)> onClick;
};