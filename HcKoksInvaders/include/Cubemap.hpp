#pragma once
#include "TextureManager.hpp"

class Cubemap {
	unsigned int gl_texID;
public:
	Cubemap();
	unsigned int getGlID();
};