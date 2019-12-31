#pragma once
#include <glm/vec3.hpp>

class Game;

class StarShip {
	Game* const m_gamePtr;

	static const int maxAmmoSMG;
	static const int maxAmmoShotgun;
	static const int maxRocketAmmo;

	int ammoSMG;
	int ammoShotgun;
	int RocketAmmo;
public:
	StarShip(Game * const game);

	void updateOnUserInput(float deltaSeconds);
	void checkBulletCollisions();

	void addSMGAmmo();
	void addShotgunAmmo();
	void addRocketAmmo();

	float getSMGAmmoPercent();
	float getShotgunAmmoPercent();
	float getRocketAmmoPercent();

	void setPos(glm::vec3 pos);
	void movePos(glm::vec3 length);

	void draw();
};