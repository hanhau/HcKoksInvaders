#include "include/BulletEmitter.hpp"
#include "include/Game.hpp"
#include <stdio.h>

BulletEmitter::BulletEmitter(Game* const gamePtr,
							 const int seed,
							 std::initializer_list<GunType> validTypes) :
	m_beGamePtr(gamePtr)
{
	int pick = seed % validTypes.size();
	m_beGunType = *(validTypes.begin() + pick);
}

BulletEmitter::BulletEmitter(Game* const gamePtr,
							 GunType initGunType) :
	m_beGamePtr(gamePtr),
	m_beGunType(initGunType)
{}

void BulletEmitter::fireIfReady(const glm::vec3 pos, const glm::vec2 orientation)
{
	glm::vec2 on = orientation;

	switch (m_beGunType) {
	case GunType::Pistol:
		{
			if (m_beClockPistol.getElapsedTime().asSeconds() >= m_beCooldownPistolSecs) {
				m_beGamePtr->addBullet(Bullet{
					pos,
					glm::vec3(on.x * m_bePistolSpeed, on.y * m_bePistolSpeed, 0.f),
					m_bePistolDamage,
					m_beBulletOwner
				});
				m_beClockPistol.restart();
			}
		}
		break;
	case GunType::Rocket:
		{
			
		}
		break;
	case GunType::Shotgun:
		{

		}
		break;
	case GunType::SMG:
		{

		}
		break;
	default:
		throw std::exception("Invalid Weapon Type");
		break;
	}
}

void BulletEmitter::setGunType(const GunType gunType) {
	m_beGunType = gunType;
}
GunType BulletEmitter::getGunType() {
	return m_beGunType;
}

BulletEmitter::~BulletEmitter() {}