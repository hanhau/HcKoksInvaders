#pragma once
#include <string>
#include <memory>
#include <glm/vec2.hpp>
#include <SFML/Window/Window.hpp>
#include "Program.hpp"

class Text {
protected:
	class impl;
	impl* m_impl;
public:
	Text(const std::string text,int pixelheight,glm::ivec2 posInPixel);

	void setText(const std::string text);
	void setPos(const glm::ivec2 posInPixel);

	glm::vec2 getSizeNDC(const sf::Window& win);
	glm::ivec2 getSizePixels(const sf::Window& win);

	void centerHorizontally(const sf::Window& win);
	void centerVertically(const sf::Window& win);

	void draw(const sf::Window& win, const Program& program,glm::vec3 col = glm::vec3(1.0f));
};