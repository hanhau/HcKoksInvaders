#include "include/EnemySpaceShipTile.hpp"

EnemySpaceShipTile::EnemySpaceShipTile(
	Game* const ref_game, int seed,
	const glm::vec3 pos,float scale
) :
	TileEntityBase(ref_game),
	m_shipPos(std::move(
		ModelPosition(
			glm::vec3(pos),
			glm::radians(90.0f),glm::vec3(1.0f,0.f,0.f),
			glm::vec3(scale)
		)
	)),
	m_pos(pos)
{

}

void EnemySpaceShipTile::update(double deltaTime) {
	
};

void EnemySpaceShipTile::draw() {

};

const ModelPosition& EnemySpaceShipTile::getSpaceshipPos() {
	return m_shipPos;
}
const glm::vec3& EnemySpaceShipTile::getPos() {
	return m_pos;
}