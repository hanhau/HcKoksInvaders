#pragma once
#include "TileEntityBase.hpp"
#include "ModelPosition.hpp"
#include <SFML/System/Clock.hpp>
#include "BulletEmitter.hpp"
#include "Bullet.hpp"

class EnemyTurretTile : public TileEntityBase, public BulletEmitter
{
	ModelPosition m_basePos;
	ModelPosition m_headPos;

	float m_health;
	const glm::vec3 m_pos;
public:
	EnemyTurretTile(Game* const ref_game,int seed,
					const glm::vec3 pos,const float scale);

	void update(double deltaTime);
	void draw();

	void letShoot(const glm::vec3 pos, const glm::vec2 orientation);
	void takeDamage(float dmg);
	float getHealth();

	const glm::vec3& getPos() const;
	ModelPosition& getBasePos();
	ModelPosition& getHeadPos();
};