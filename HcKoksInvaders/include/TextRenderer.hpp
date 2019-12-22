#pragma once
#include "Text.hpp"
#include "ProgramManager.hpp"
#include <SFML/Window.hpp>

class TextRenderer {
	unsigned int gl_vao;
	unsigned int gl_vbo;
	unsigned int gl_textureID;

	const Program& m_progRef;
public:
	TextRenderer(const ProgramManager& progMgr,const sf::Window& win);

	void draw(const Text& text) const {

	}
};