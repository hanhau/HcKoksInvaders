#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include "TextureManager.hpp"
#include <Program.hpp>

class AmmunitionIcon
{
	// setup
	sf::Vector2f m_position;
	float m_radius;
	sf::Color m_col;

	int m_minDotsAmount;
	int m_maxDotsAmount;

	// OpenGL
	unsigned int gl_vao;
	unsigned int gl_vbo;
	unsigned int gl_textureID;
public:
	AmmunitionIcon(
		const std::string pathIcon, 
		sf::Color outlineColor, 
		const float radius, sf::Vector2f pos,
		TextureManager& texMgr,
		const sf::Window& window
	);

	void draw(float percentageFull,const Program& program);
};