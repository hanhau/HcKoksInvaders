#pragma once
#include <array>
#include <SFML/Audio.hpp>

class SoundQueue
{
	static std::array<sf::Sound,200>* m_sounds;
public:
	static sf::Sound& add(const sf::SoundBuffer& soundbuf,const float pitch = 1.0f);
	static int getActivePlayingSounds();

	static void waitTillEverythingCompleted();
};