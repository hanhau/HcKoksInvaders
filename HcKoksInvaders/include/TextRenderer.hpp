#pragma once
#include <string>
#include <memory>

#include <glm/vec2.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>

class TextRenderer {
	static sf::RenderTexture sf_renderTex;
	sf::Text sf_text;
	sf::Font sf_font;
public:
	static void drawToOverlay();

	static void init(const sf::Window& win);

	static void drawToOverlay();
	static void clearOverlay();
	static void drawOverlay();
};