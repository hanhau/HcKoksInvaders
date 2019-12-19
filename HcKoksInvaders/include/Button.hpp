#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include <array>

class Button
{
private:
	static sf::Font m_font;
	static bool m_fontLoaded;

	std::string m_title;
	sf::Vector2f m_size;
	sf::Vector2f m_position;
	sf::Text m_text;

	unsigned int gl_vao;
	unsigned int gl_vbo;
public:
	Button(std::string title, sf::Vector2f pos, sf::Vector2f size);

	void draw(sf::RenderWindow& win,sf::Shader * shader) const;
	bool containsPoint(const sf::Vector2f& p) const;

	std::function<void()> onHover;
	bool onHoverActive;
	std::function<void()> onClick;
};