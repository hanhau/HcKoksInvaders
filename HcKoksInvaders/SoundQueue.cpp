#include "include/SoundQueue.hpp"

void _SoundQueue::cleanPlayedSounds() {
	if (size() == 0)
		return;

	if (front().getStatus() == sf::Sound::Stopped) {
		pop_front();
		cleanPlayedSounds();
	}
}