#pragma once
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>

class SoundBufferManager {
	std::map<std::string, sf::SoundBuffer> m_soundBuffers;
	void add(std::string path);

public:
	SoundBufferManager();

	sf::SoundBuffer& get(std::string path);
	bool exists(std::string path);
};