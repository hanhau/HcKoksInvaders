#pragma once
#include <deque>
#include <SFML/Audio/Sound.hpp>

static class _SoundQueue : 
	public std::deque<sf::Sound> 
{
public:
	void cleanPlayedSounds();
} SoundQueue;