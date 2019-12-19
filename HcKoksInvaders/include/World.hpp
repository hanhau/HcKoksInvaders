#pragma once
#include <SFML/Graphics.hpp>

class GameWorld {
private:


public:
	static const int TileSize;
	static const int MapWidthInTiles;
	static const int MinMapHeightInTiles;

	void init(int mapheight);
	void update(double deltaTime);
	void draw(sf::RenderWindow &rw);
};