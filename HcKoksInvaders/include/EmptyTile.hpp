#pragma once
#include "TileEntityBase.hpp"

class EmptyTile : public TileEntityBase 
{
public:
	EmptyTile(Game * const ref_game);

	void update(double deltaTIme);
	void draw();
};
