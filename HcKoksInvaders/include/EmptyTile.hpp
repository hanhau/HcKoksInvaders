#pragma once
#include "TileEntityBase.hpp"

class EmptyTile : TileEntityBase 
{
public:
	EmptyTile(Game * const ref_game);

	void update(double deltaTIme);
	void draw();
};
