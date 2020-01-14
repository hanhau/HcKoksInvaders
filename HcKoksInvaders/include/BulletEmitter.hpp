#pragma once
#include <initializer_list>
#include <SFML/System/Clock.hpp>
#include <glm/vec2.hpp>
#include <Bullet.hpp>

class Game;

enum class GunType {
	Pistol, SMG, Shotgun, Rocket
};

class BulletEmitter {
protected:
	Game* const m_beGamePtr;
	GunType m_beGunType;
	sf::Clock m_beLastShot;

	sf::Clock m_beClockPistol;
	sf::Clock m_beClockSMG;
	sf::Clock m_beClockShotgun;
	sf::Clock m_beClockRocket;

	// Specific Instance Properties
	float m_beCooldownPistolSecs;
	float m_beCooldownSMGSecs;
	float m_beCooldownShotgunSecs;
	float m_beCooldownRocketSecs;

	float m_bePistolSpeed;
	float m_beSMGSpeed;
	float m_beShotgunSpeed;
	float m_beRocketSpeed;

	float m_bePistolDamage;
	float m_beSMGDamage;
	float m_beShotgunDamage;
	float m_beRocketDamage;

	Bullet::Owner m_beBulletOwner;
public:
	BulletEmitter(
		Game* const gamePtr,
		const int seed,
		std::initializer_list<GunType> validTypes
	);
	BulletEmitter(
		Game* const gamePtr,
		GunType initGunType
	);

	void fireIfReady(const glm::vec3 pos, const glm::vec2 orientation);

	void setGunType(const GunType gunType);
	GunType getGunType();

	virtual ~BulletEmitter();
};