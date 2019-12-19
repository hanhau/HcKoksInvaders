#pragma once
#include <map>
#include <SFML/Graphics.hpp>

class TextureManager {
	std::map<std::string, sf::Texture> m_textures;
	void add(std::string path);
public:
	TextureManager();

	const sf::Texture& get(std::string path);
	bool exists(std::string path);
};
