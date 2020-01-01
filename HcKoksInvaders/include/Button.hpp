#pragma once
#include <string>
#include <functional>
#include <array>
#include <glm/vec2.hpp>
#include "ProgramManager.hpp"
#include "Text.hpp"
#include <SFML/Window/Window.hpp>

class Button : private Text
{
private:
	std::string m_title;
	glm::ivec2 m_position;
	glm::vec2 m_pixelSize;

	unsigned int gl_vao;
	unsigned int gl_vbo;
public:
	Button(const sf::Window& win,const std::string title, const glm::ivec2 posPixel, int fontHeight);

	void draw(const sf::Window& window,const ProgramManager& progMgr);
	bool containsPoint(const glm::ivec2 pointInPixel) const;

	std::function<void(Button&)> onHoverEnter;
	std::function<void(Button&)> onHoverLeave;
	std::function<void(Button&)> onClick;
};