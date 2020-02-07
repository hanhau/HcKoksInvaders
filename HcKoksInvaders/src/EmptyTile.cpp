#include "../include/EmptyTile.hpp"

EmptyTile::EmptyTile(Game * const ref_game) :
	TileEntityBase(ref_game)
{}

void EmptyTile::draw() {}

void EmptyTile::update(double deltaTime) {}