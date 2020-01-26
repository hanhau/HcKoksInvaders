#pragma once
#include <string>
#include <SFML/Window/Window.hpp>
#include "TextureManager.hpp"
#include "ProgramManager.hpp"

class AmmunitionIcon
{
	// setup
	const glm::ivec2 m_position;
	const float m_radius;
	const glm::vec4 m_col;

	int m_minDotsAmount;
	int m_maxDotsAmount;

	// OpenGL
	unsigned int gl_vao;
	unsigned int gl_vbo;
	unsigned int gl_textureID;
public:
	AmmunitionIcon(
		const std::string pathIcon, 
		glm::vec4 outlineColor, 
		const int radius, glm::ivec2 pos,
		const sf::Window& window
	);

	void draw(
		const sf::Window& win,
		float percentageFull, // 0.0f (empty) - 100.f (full)
		const std::string slotTextStr,
		bool active,
		float reloadProgress // 0.0f (needsTime) - 1.0f (ready2Shoot)
	);
};