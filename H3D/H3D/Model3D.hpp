#pragma once

#include <assimp/scene.h>

class Model3D {
	
public:
	bool loadFromFile();
	bool loadFromMemory();
};