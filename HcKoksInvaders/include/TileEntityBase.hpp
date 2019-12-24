#pragma once
#include "Game.hpp"
#include <SFML/System/Vector3.hpp>

class TileEntityBase {
protected:
	Game * const m_gameRef;
public:
	TileEntityBase(Game* const game);
	TileEntityBase();

	virtual void update(double deltaTime);
	virtual void draw();
	virtual ~TileEntityBase();
};