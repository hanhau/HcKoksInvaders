#include "../include/SoundQueue.hpp"
#include <thread>

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

void SoundQueue::waitTillEverythingCompleted() {
	if (m_sounds == nullptr)
		return;

	bool running;
	do {
		running = false;
		for (const auto& iter : *m_sounds)
			if (iter.getStatus() == sf::Sound::Playing)
				running = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	} while (running);
}