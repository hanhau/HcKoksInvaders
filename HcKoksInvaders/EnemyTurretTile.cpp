#include "include/EnemyTurretTile.hpp"

EnemyTurretTile::EnemyTurretTile(Game* const ref_game) :
	TileEntityBase(ref_game)
{

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

const ModelPosition& EnemyTurretTile::getBasePos() {
	return m_basePos;
}
const ModelPosition& EnemyTurretTile::getHeadPos() {
	return m_headPos;
}