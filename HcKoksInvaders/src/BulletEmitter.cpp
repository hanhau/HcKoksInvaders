#include "../include/BulletEmitter.hpp"
#include "../include/Game.hpp"
#include "../include/SoundQueue.hpp"
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
	
	static const glm::mat4 rotateBitLeft = glm::rotate(glm::identity<glm::mat4>(),glm::radians(10.f),glm::vec3(0,0,1));
	static const glm::mat4 rotateBitRight = glm::rotate(glm::identity<glm::mat4>(),glm::radians(-10.f),glm::vec3(0,0,1));

	bool shot = false;

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
				
				shot = true;
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
			if (m_beClockShotgun.getElapsedTime().asSeconds() >= m_beCooldownShotgunSecs) {
				m_beGamePtr->addBullet(Bullet{
					pos,
					glm::vec3(on.x * m_beShotgunSpeed, on.y * m_beShotgunSpeed, 0.f),
					m_beShotgunDamage,
					m_beBulletOwner
				});
				auto onlb = (rotateBitLeft * glm::vec4(on,0,1.0));
				auto onrb = (rotateBitRight * glm::vec4(on,0,1.0));
				m_beGamePtr->addBullet(Bullet{
					pos,
					glm::vec3(onlb.x * m_beShotgunSpeed, onlb.y * m_beShotgunSpeed, 0.f),
					m_beShotgunDamage,
					m_beBulletOwner
				});
				m_beGamePtr->addBullet(Bullet{
					pos,
					glm::vec3(onrb.x * m_beShotgunSpeed, onrb.y * m_beShotgunSpeed, 0.f),
					m_beShotgunDamage,
					m_beBulletOwner
				});

				shot = true;
				m_beClockShotgun.restart();
			}
		}
		break;
	case GunType::SMG:
		{
			if (m_beClockSMG.getElapsedTime().asSeconds() >= m_beCooldownSMGSecs) {
				m_beGamePtr->addBullet(Bullet{
					pos,
					glm::vec3(on.x * m_beSMGSpeed, on.y * m_beSMGSpeed, 0.f),
					m_beSMGDamage,
					m_beBulletOwner
				});

				shot = true;
				m_beClockSMG.restart();
			}
		}
		break;
	default:
		throw std::exception("Invalid Weapon Type");
		break;
	}

	if (shot) {
		SoundQueue::add(
			m_beGamePtr->getSoundBufferManager().get("res/audio/gunEmitter.wav"), 
			0.9f + sinf(m_beClockSMG.getElapsedTime().asMicroseconds()) * 0.1f
		);
	}
}

void BulletEmitter::setGunType(const GunType gunType) {
	m_beGunType = gunType;
}
GunType BulletEmitter::getGunType() {
	return m_beGunType;
}

BulletEmitter::~BulletEmitter() {}