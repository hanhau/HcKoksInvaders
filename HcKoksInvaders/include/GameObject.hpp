#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
private:

public:
	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& rw);
	virtual void onPickup();
	virtual void onPlayerHit();
};