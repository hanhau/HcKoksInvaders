#pragma once
#include "Game.hpp"
#include <SFML/System/Vector3.hpp>

class TileEntityBase {
protected:
	Game * const m_gameRef;
public:
	inline TileEntityBase(Game * const game) :
		m_gameRef(game)
	{}
		
	inline virtual void update(double deltaTime) {};
	inline virtual void draw() {};
	inline virtual ~TileEntityBase() {};
};