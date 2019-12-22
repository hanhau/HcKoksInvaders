#pragma once
#include <Program.hpp>
#include <TextureManager.hpp>

class StarBackground {
	unsigned int gl_vao;
	unsigned int gl_vbo;
public:
	StarBackground();
	~StarBackground();

	void draw(const Program& program,float pastTime) const;
};