#pragma once
#include <glm/vec2.hpp>
#include <string>

struct GameLaunchOptions {
	glm::ivec2 res = glm::ivec2(100,100);
	bool fullscreen = false;

	std::string password;
	std::string login;

	int userid = 0;
	bool exit = false;

	void loadFromFileOrDefault();
	void saveToFile();
};