#pragma once
#include <string>
#include <memory>

#include <glm/vec2.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "Program.hpp"

class TextRenderer {
	static sf::RenderTexture* sf_renderTex;
	static unsigned int gl_vao;
	static unsigned int gl_vbo;

	static Program* m_program;
	static Shader* m_vertShader;
	static Shader* m_fragShader;
	static sf::Window * m_win;
public:
	static void init(const sf::Window& win);

	static void drawToOverlay(sf::Text& text);
	static void clearOverlay();
	static void drawOverlay();
};