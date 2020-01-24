#include "include/TextureManager.hpp"
#include <iostream>
#include <GL\glew.h>
#include <fstream>
#include <vector>

const std::vector<std::string> TextureManager::m_rmPreloadPaths({
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

void _preloadFunc(const std::vector<std::string>& loadPaths,
				 std::map<std::string, PreloadData>& pDataMap,
				 std::map<const std::string, Texture> &textures) 
{
	pDataMap.clear();

	for (const auto& path : loadPaths) {
		std::ifstream file(path, std::ios::in | std::ios::ate | std::ios::binary);
		
		size_t fileLength = file.tellg();
		file.seekg(0);

		pDataMap[path] = PreloadData();
		pDataMap[path].m_dataLength = fileLength;
		pDataMap[path].m_path = path;
		pDataMap[path].m_data = std::unique_ptr<uint8_t>(new uint8_t[fileLength]);

		file.read((char*)pDataMap[path].m_data.get(), fileLength);
	}

	for (const auto& pData: pDataMap) {
		textures.emplace(pData.first,Texture());
		textures[pData.first].loadFromMemory(
			pData.second.m_data.get(), 
			pData.second.m_dataLength
		);
	}

	pDataMap.clear();
}

void TextureManager::preloadToMemory() {
	m_rmPreloadThread = std::thread(
		_preloadFunc,
		std::ref(m_rmPreloadPaths), std::ref(m_rmPreloadData), std::ref(m_textures)
	);
}

void TextureManager::waitForMemoryPreload() {
	m_rmPreloadThread.join();
}

void TextureManager::init() {
	for (auto& tex : m_textures) {
		tex.second.uploadToGl();
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