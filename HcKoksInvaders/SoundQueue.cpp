#include "include/SoundQueue.hpp"

std::array<sf::Sound, 200>* SoundQueue::m_sounds;

sf::Sound& SoundQueue::add(const sf::SoundBuffer& soundbuf, const float pitch)
{
	if (m_sounds == nullptr)
		m_sounds = new std::array<sf::Sound,200>();

	for (auto& iter : *m_sounds) {
		if (iter.getStatus() == sf::Sound::Status::Stopped) {
			iter.setBuffer(soundbuf);
			iter.setPitch(pitch);
			iter.play();

			return iter;
		}
	}
}

int SoundQueue::getActivePlayingSounds() {
	if (m_sounds == nullptr)
		return 0;

	int count = 0;
	for (auto& iter : *m_sounds)
		if (iter.getStatus() == sf::Sound::Status::Playing)
			count++;

	return count;
}