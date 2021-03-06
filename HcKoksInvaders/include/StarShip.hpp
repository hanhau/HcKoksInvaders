#pragma once
#include <glm/vec3.hpp>
#include "InstanceBuffer.hpp"
#include "Game.hpp"
#include "Camera.hpp"
#include "Cubemap.hpp"
#include <SFML/System/Clock.hpp>

enum class WeaponType {
	Pistol,
	Shotgun,
	Rocket,
	SMG
};

class StarShip {
	Game& m_gameRef;
	void processShoot();
	glm::vec3 m_pos;
	float m_health;

	// Max Ammo of a Weapon Type
	static const int maxAmmoSMG;
	static const int maxAmmoShotgun;
	static const int maxAmmoRocket;

	// Cooldown Times in Seconds
	static const float coolDownPistol;
	static const float coolDownRocket;
	static const float coolDownShotgun;
	static const float coolDownSMG;

	// Current Ammo
	int m_ammoSMG;
	int m_ammoShotgun;
	int m_ammoRocket;

	// Cooldown Clocks of Weapons
	sf::Clock m_clockSMG;
	sf::Clock m_clockShotgun;
	sf::Clock m_clockRocket;
	sf::Clock m_clockPistol;

	// Currently selected Weapon
	WeaponType m_activeWeapon;

	static const float m_scale;
public:
	StarShip(Game& game);

	void updateOnUserInput(float deltaSeconds);

	void addSMGAmmo(int shots);
	void addShotgunAmmo(int shots);
	void addRocketAmmo(int shots);

	const float getWeaponAmmoPercent(const WeaponType wt) const;
	const float getWeaponReloadProgress(const WeaponType wt) const;

	const int getHealth();
	void setHealth(int health);

	const glm::vec3& getPos() const;
	void setPos(const glm::vec3 pos);

	WeaponType getWeaponType();

	void draw(const Camera& camera, Cubemap& cubemap);
	void handleBullets(std::list<Bullet>& bullets);
};