#pragma once
#include <glm/vec2.hpp>
#include <string>

struct GameLaunchOptions {
	glm::ivec2 res;
	bool fullscreen;

	bool online;
	std::string login;
	std::string password;

	bool exit;
};