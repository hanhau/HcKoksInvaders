#include "include/EnemySpaceShipTile.hpp"

EnemySpaceShipTile::EnemySpaceShipTile(Game* const ref_game, int seed) :
	TileEntityBase(ref_game)
{

}

void EnemySpaceShipTile::update(double deltaTime) {
	
};

void EnemySpaceShipTile::draw() {

};

const ModelPosition& EnemySpaceShipTile::getSpaceshipPos() {
	return m_shipPos;
}