#pragma once
#include <map>
#include <thread>
#include "PreloadData.hpp"
#include "Texture.hpp"
#include "ResourceManagerBase.hpp"

class TextureManager : public ResourceMangerBase {	
	static std::map<const std::string, Texture> m_textures;
	static void add(std::string path);

	~TextureManager() {}
public:
	static void init();

	static const Texture& get(const std::string path);
	static bool exists(std::string path);
};
