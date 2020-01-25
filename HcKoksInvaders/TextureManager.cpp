#include "include/TextureManager.hpp"
#include <iostream>
#include <GL\glew.h>
#include <fstream>
#include <vector>
#include <string>

_TextureManager TextureManager;

_TextureManager::_TextureManager() :
	ResourceManagerBase({
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
	})
{}

void _preloadFunc(const std::vector<std::string>& loadPaths,
				  std::map<std::string, PreloadData>& pDataMap,
				  std::map<std::string, Texture> * const textures) 
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
		textures->emplace(pData.first,Texture());
		(*textures)[pData.first].loadFromMemory(
			pData.second.m_data.get(), 
			pData.second.m_dataLength
		);
	}

	pDataMap.clear();
}

void _TextureManager::preloadToMemory() {
	m_rmPreloadThread = std::thread(
		_preloadFunc,
		std::ref(m_rmPreloadPaths), std::ref(m_rmPreloadData), &this->m_textures
	);
	std::cout << m_rmPreloadThread.joinable() << "\n";
}

void _TextureManager::waitForMemoryPreload() {
	if(m_rmPreloadThread.joinable())
		m_rmPreloadThread.join();
}

void _TextureManager::init() {
	for (auto& tex : m_textures) {
		tex.second.uploadToGl();
	}
}
void _TextureManager::cleanMemory() {
	for (auto& tex : m_textures) {
		tex.second.cleanBuffer();
	}
}

const Texture& _TextureManager::get(const std::string path) {
	try {
		if (m_textures.size() == 0)
			throw "No textures loaded @ " + std::string(__FUNCTION__);

		if (m_textures.find(path) == m_textures.end())
			throw "Invalid path @ " + std::string(__FUNCTION__);

		return m_textures.at(path);
	}
	catch (std::string e) {
		std::cout << e << std::endl;		
	}	
}

bool _TextureManager::exists(std::string path) {
	return m_textures.find(path) != m_textures.end();
}