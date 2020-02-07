#include "../include/EnemyTurretTile.hpp"
#include "../include/Bullet.hpp"

EnemyTurretTile::EnemyTurretTile(Game* const ref_game,int seed,
								 const glm::vec3 pos,const float scale) :
	TileEntityBase(ref_game),
	BulletEmitter(ref_game, seed, {GunType::Pistol,GunType::SMG,GunType::Shotgun}),
	m_pos(pos)
{
	m_health = 100.f;
	m_basePos = std::move(ModelPosition(
		pos, glm::radians(90.f), glm::vec3(1.f,0.f,0.f), glm::vec3(scale)
	));
	m_headPos = std::move(ModelPosition(
		pos, glm::radians(90.f), glm::vec3(1.f,0.f,0.f), glm::vec3(scale)
	));

	// Bullet Emitter Properties
	m_beBulletOwner = Bullet::Owner::Enemy;

	m_beCooldownPistolSecs = 3.f;
	m_bePistolDamage = 10.f;
	m_bePistolSpeed = 1.1f;

	m_beCooldownShotgunSecs = 3.2f;
	m_beShotgunDamage = 12.f;
	m_beShotgunSpeed = 1.4f;

	m_beCooldownSMGSecs = 2.0f;
	m_beSMGDamage = 10.f;
	m_beSMGSpeed = 1.0f;
}

void EnemyTurretTile::update(double deltaTime) 
{
	
}

void EnemyTurretTile::takeDamage(float dmg) {
	m_health -= dmg;
}
float EnemyTurretTile::getHealth() {
	return m_health;
}

void EnemyTurretTile::draw() 
{

}

void EnemyTurretTile::letShoot(const glm::vec3 pos, const glm::vec2 orientation) {
	fireIfReady(pos, orientation);
}

const glm::vec3& EnemyTurretTile::getPos() const {
	return m_pos;
}

ModelPosition& EnemyTurretTile::getBasePos() {
	return m_basePos;
}
ModelPosition& EnemyTurretTile::getHeadPos() {
	return m_headPos;
}