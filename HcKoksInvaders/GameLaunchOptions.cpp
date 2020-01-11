#include "include/GameLaunchOptions.hpp"
#include <filesystem>
#include <fstream>

const std::string _filepath = "defaultSettings.lbs";

void GameLaunchOptions::loadFromFileOrDefault() {
	if (!std::filesystem::exists(_filepath)) {
		res = glm::ivec2(640, 960);
		fullscreen = false;

		password = login = "";
		userid = 0;

		this->exit = false;
		return;
	}

	std::ifstream file(_filepath,std::ios::binary | std::ios::in);

	file.read((char*)&fullscreen, sizeof(bool));

	size_t lenLogin = 0;
	file.read((char*)&lenLogin, sizeof(size_t));
	login.resize(lenLogin,'\0');
	file.read(login.data(),lenLogin);

	size_t lenPassword = 0;
	file.read((char*)&lenPassword, sizeof(size_t));
	password.resize(lenPassword, '\0');
	file.read(password.data(), lenPassword);
}

void GameLaunchOptions::saveToFile() {
	std::ofstream file(_filepath, std::ios::binary | std::ios::trunc | std::ios::out);

	// Resolution & Fullscreen
	file.write((const char*)&fullscreen, sizeof(bool));

	// Login Information
	size_t lenLogin = login.size();
	file.write((const char*)&lenLogin, sizeof(size_t));
	file.write(login.c_str(), login.size());

	size_t lenPassword = password.size();
	file.write((const char*)&lenPassword, sizeof(size_t));
	file.write(password.c_str(), password.size());
}