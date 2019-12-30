#pragma once
#include <string>
#include <memory>
#include <glm/vec2.hpp>
#include <SFML/Window/Window.hpp>
#include "Program.hpp"

class Text {
	class impl;
	impl* m_impl;
public:
	Text(const std::string text,int pixelheight,glm::ivec2 posInPixel);

	void setText(const std::string text);
	void setPos(const glm::ivec2 posInPixel);

	void draw(const sf::Window& win, const Program& program);
};