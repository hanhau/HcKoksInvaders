#include "include/SoundBufferManager.hpp"
#include <iostream>

void SoundBufferManager::add(std::string path)
{
	try {
		if (exists(path))
			throw "Sound does already exists @ SoundBufferManager::add(path)";

		m_soundBuffers.emplace(path,sf::SoundBuffer());

		if (!m_soundBuffers[path].loadFromFile(path))
			throw "Unable to load Sound @ SoundBufferManager::add(path)";
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

SoundBufferManager::SoundBufferManager() {
	add("res/audio/select.flac");
}

sf::SoundBuffer& SoundBufferManager::get(std::string path)
{
	try {
		if (!exists(path))
			throw "SoundBuffer does not exist @ SoundBufferManager::get(path)";

		return m_soundBuffers[path];
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		throw;
	}
}

bool SoundBufferManager::exists(std::string path) {
	return m_soundBuffers.find(path) != m_soundBuffers.end();
}
