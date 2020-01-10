#pragma once
#include <glm/vec2.hpp>
#include <string>

struct GameLaunchOptions {
	glm::ivec2 res = glm::ivec2();
	bool fullscreen = false;

	std::string token = "";
	bool exit = false;
};