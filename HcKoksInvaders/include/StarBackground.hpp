#pragma once
#include <Program.hpp>
#include <TextureManager.hpp>
#include <SFML/Window/Window.hpp>

class StarBackground {
	unsigned int gl_vao;
	unsigned int gl_vbo;
public:
	StarBackground();
	~StarBackground();

	void draw(const sf::Window& window,const Program& program,float pastTime) const;
};