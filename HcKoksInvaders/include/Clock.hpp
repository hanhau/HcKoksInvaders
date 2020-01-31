#pragma once
#include <memory>

class Clock {
	struct impl;
	std::unique_ptr<impl> m_impl;
public:
	void restart();
	void pause();
	void resume();

	uint64_t getElapsedMicroseconds();
	double	 getElapsedSeconds();
	double	 getElapsedMilliseconds();
};

// 2DO ..... ich möchte von SFML wegkommen