#pragma once
#include <map>
#include <SFML/Graphics.hpp>

class TextureManager {
	std::map<const std::string, sf::Texture> m_textures;
	void add(std::string path);
public:
	TextureManager();

	const sf::Texture& get(const std::string path) const;
	bool exists(std::string path) const;
};
