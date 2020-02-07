#include "../include/SoundBufferManager.hpp"
#include <iostream>

void SoundBufferManager::add(std::string path)
{
	try {
		if (exists(path))
			throw std::string("Sound does already exists @ " + std::string(__FUNCTION__));

		m_soundBuffers.emplace(path,sf::SoundBuffer());

		if (!m_soundBuffers[path].loadFromFile(path))
			throw std::string("Unable to load Sound @ " + std::string(__FUNCTION__));
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}
}

SoundBufferManager::SoundBufferManager() {
	add("res/audio/select.flac");
	add("res/audio/hcDankeBye.wav");

	add("res/audio/gunShotgun.wav");
	add("res/audio/gunPistol.wav");
	add("res/audio/gunSMG.wav");
	add("res/audio/gunEmitter.wav");
	add("res/audio/playershipHit.wav");
}

const sf::SoundBuffer& SoundBufferManager::get(std::string path) const
{
	try {
		if (!exists(path))
			throw "SoundBuffer does not exist @ SoundBufferManager::get(path)";

		return m_soundBuffers.at(path);
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		throw;
	}
}

bool SoundBufferManager::exists(std::string path) const {
	return m_soundBuffers.find(path) != m_soundBuffers.end();
}
