#pragma once
#include <map>
#include <thread>
#include "PreloadData.hpp"
#include "Texture.hpp"
#include "ResourceManagerBase.hpp"

class _TextureManager : public ResourceManagerBase {	
	std::map<std::string, Texture> m_textures;
public:
	_TextureManager();

	void init();
	void cleanMemory();

	void preloadToMemory();
	void waitForMemoryPreload();

	const Texture& get(const std::string path);
	bool exists(std::string path);
};

extern _TextureManager TextureManager;