#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

class Text {
	std::string m_text;
	float m_height;
	sf::Vector2f m_pos;

public:
	Text(const std::string text, float height, sf::Vector2f pos);
};