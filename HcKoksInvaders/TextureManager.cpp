#include "include/TextureManager.hpp"
#include <iostream>
#include <GL\glew.h>
#include <fstream>
#include <vector>

const std::vector<std::string> LOAD_PATHS({
	"res/images/icon_munition_pistol.png",
	"res/images/icon_munition_rocket.png",
	"res/images/icon_munition_shotgun.png",
	"res/images/icon_munition_smg.png",

	"res/images/cubemap/front.png",
	"res/images/cubemap/back.png",
	"res/images/cubemap/left.png",
	"res/images/cubemap/right.png",
	"res/images/cubemap/top.png",
	"res/images/cubemap/bot.png",

	"res/models/ship1_diffuse.png",
	"res/models/ship1_glossy.png",
	"res/models/vengabus_diffuse.png",
	"res/models/vengabus_glossy.png",

	"res/models/vengabus_hq_diffuse.png",
	"res/models/vengabus_hq_glossy.png",

	"res/models/finger_diffuse.png",
	"res/models/finger_glossy.png",

	"res/models/money_diffuse.png",
	"res/models/money_glossy.png",

	"res/models/turret_base_diffuse.png",
	"res/models/turret_base_glossy.png",

	"res/models/turret_head_diffuse.png",
	"res/models/turret_head_glossy.png"
});

std::map<std::string, PreloadData>   TextureManager::m_rmPreloadData;
std::thread						     TextureManager::m_rmPreloadThread;
std::map<const std::string, Texture> TextureManager::m_textures;

void preloadFunc(std::vector<std::string>& loadPaths,
				 std::map<std::string, PreloadData>& pData) 
{
	for (const auto& path : loadPaths) {
		std::ifstream file(path, std::ios::in | std::ios::ate | std::ios::binary);
		
		size_t fileLength = file.tellg();
		file.seekg(0);


	}
}

void TextureManager::preloadToMemory() {
	m_preloadThread = std::thread(&preloadFunc);
}

void TextureManager::waitForMemoryPreload() {
	m_preloadThread.join();
}

void TextureManager::init() {
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

		Texture temp_tex;
		const PreloadData& pData = m_preloadData[path];
		if (!temp_tex.loadFromMemory(pData.m_data.get(),pData.m_dataLength)) {
			throw "Error loading file @ " + std::string(__FUNCSIG__);
			return;
		}

		m_textures.emplace(path, std::move(temp_tex));
	}
	catch (std::string e) {
		std::cout << e << std::endl;
	}
}

const Texture& TextureManager::get(const std::string path) {
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

bool TextureManager::exists(std::string path) {
	return m_textures.find(path) != m_textures.end();
}