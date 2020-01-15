#include "include/TextureManager.hpp"
#include <iostream>
#include <GL\glew.h>

TextureManager::TextureManager() {
	add("res/images/icon_munition_pistol.png");
	add("res/images/icon_munition_rocket.png");
	add("res/images/icon_munition_shotgun.png");
	add("res/images/icon_munition_smg.png");

	add("res/images/cubemap/front.png");
	add("res/images/cubemap/back.png");
	add("res/images/cubemap/left.png");
	add("res/images/cubemap/right.png");
	add("res/images/cubemap/top.png");
	add("res/images/cubemap/bot.png");

	add("res/models/ship1_diffuse.png");
	add("res/models/ship1_glossy.png");

	add("res/models/vengabus_diffuse.png");
	add("res/models/vengabus_glossy.png");

	add("res/models/vengabus_hq_diffuse.png");
	add("res/models/vengabus_hq_glossy.png");

	add("res/models/finger_diffuse.png");
	add("res/models/finger_glossy.png");

	add("res/models/money_diffuse.png");
	add("res/models/money_glossy.png");

	add("res/models/turret_base_diffuse.png");
	add("res/models/turret_base_glossy.png");
	
	add("res/models/turret_head_diffuse.png");
	add("res/models/turret_head_glossy.png");
}

void TextureManager::add(std::string path) {
	try {
		if (exists(path))
			throw "Texture already added @ " + std::string(__FUNCSIG__);

		sf::Texture temp_tex;
		if (!temp_tex.loadFromFile(path)) {
			throw "Error loading file @ " + std::string(__FUNCSIG__);
			return;
		}
		temp_tex.setSmooth(true);

		m_textures.emplace(path, std::move(temp_tex));
	}
	catch (std::string e) {
		std::cout << e << std::endl;
	}
}

const sf::Texture& TextureManager::get(const std::string path) const {
	try {
		if (m_textures.find(path) == m_textures.end())
			throw "Invalid path @ " + std::string(__FUNCSIG__);

		return m_textures.at(path);
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		throw;
	}	
}

bool TextureManager::exists(std::string path) const {
	return m_textures.find(path) != m_textures.end();
}