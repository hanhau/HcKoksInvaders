#pragma once
#include "TileEntityBase.hpp"
#include <glm/vec3.hpp>

class EnemySpaceShipTile : public TileEntityBase
{
private:
	ModelPosition m_shipPos;
	glm::vec3 m_pos;
	float m_health;
	const int m_seed;

	enum class MovementPattern {
		Circle, Wave, Straight
	};
	MovementPattern m_movementPattern;
public:
	EnemySpaceShipTile(
		Game* const ref_game, 
		int seed,
		const glm::vec3 pos,
		float scale
	);

	void update(double deltaTime);
	void draw();

	void takeDamage(float dmg);
	float getHealth();

	const ModelPosition& getSpaceshipPos();
	const glm::vec3& getPos();
};