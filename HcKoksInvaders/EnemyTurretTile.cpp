#include "include/EnemyTurretTile.hpp"

EnemyTurretTile::EnemyTurretTile(Game* const ref_game,int seed,
								 const glm::vec3 pos,const float scale) :
	TileEntityBase(ref_game),
	m_pos(pos)
{
	switch (seed % 3) {
	case 0: m_gunType = GunType::Pistol; break;
	case 1: m_gunType = GunType::Shotgun; break;
	case 2: m_gunType = GunType::SMG; break;
	default: m_gunType = GunType::Pistol; break;
	}
	m_health = 100.f;

	m_basePos = std::move(ModelPosition(
		pos, glm::radians(90.f), glm::vec3(1.f,0.f,0.f), glm::vec3(scale)
	));
	m_headPos = std::move(ModelPosition(
		pos, glm::radians(90.f), glm::vec3(1.f,0.f,0.f), glm::vec3(scale)
	));
}

void EnemyTurretTile::fire()
{

}

void EnemyTurretTile::firePistolShot(std::vector<Bullet>& bullets)
{

}

void EnemyTurretTile::fireSMGShot(std::vector<Bullet>& bullets)
{

}

void EnemyTurretTile::fireShotgunShot(std::vector<Bullet>& bullets)
{

}

void EnemyTurretTile::update(double deltaTime) 
{

}

void EnemyTurretTile::draw() 
{

}

const glm::vec3 EnemyTurretTile::getPos() const {
	return m_pos;
}

ModelPosition& EnemyTurretTile::getBasePos() {
	return m_basePos;
}
ModelPosition& EnemyTurretTile::getHeadPos() {
	return m_headPos;
}