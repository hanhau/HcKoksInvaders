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
	m_pos(pos),
	m_health(100.f)
{}

void EnemySpaceShipTile::update(double deltaTime) {
	
};

void EnemySpaceShipTile::draw() {

};

void EnemySpaceShipTile::takeDamage(float dmg) {
	m_health -= dmg;
}
float EnemySpaceShipTile::getHealth() {
	return m_health;
}

const ModelPosition& EnemySpaceShipTile::getSpaceshipPos() {
	return m_shipPos;
}
const glm::vec3& EnemySpaceShipTile::getPos() {
	return m_pos;
}