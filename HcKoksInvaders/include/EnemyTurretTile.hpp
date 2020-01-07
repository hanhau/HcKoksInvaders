#pragma once
#include "TileEntityBase.hpp"
#include "ModelPosition.hpp"
#include <SFML/System/Clock.hpp>
#include "Bullet.hpp"

class EnemyTurretTile : public TileEntityBase
{
	ModelPosition m_basePos;
	ModelPosition m_headPos;

	enum class GunType {
		Pistol, SMG, Shotgun
	};

	sf::Clock m_lastShot;
	sf::Clock m_bulletInterval;
	float m_health;
	GunType m_gunType;
	const glm::vec3 m_pos;

	void fire();
	void firePistolShot(std::vector<Bullet>& bullets);
	void fireSMGShot(std::vector<Bullet>& bullets);
	void fireShotgunShot(std::vector<Bullet>& bullets);

public:
	EnemyTurretTile(Game* const ref_game,int seed,
					const glm::vec3 pos,const float scale);

	void update(double deltaTime);
	void draw();

	void takeDamage(float dmg);
	float getHealth();

	const glm::vec3 getPos() const;
	ModelPosition& getBasePos();
	ModelPosition& getHeadPos();
};