#pragma once
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>

class SoundBufferManager {
	std::map<std::string, sf::SoundBuffer> m_soundBuffers;
	void add(std::string path);

public:
	SoundBufferManager();

	const sf::SoundBuffer& get(std::string path) const;
	bool exists(std::string path) const;
};