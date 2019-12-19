#pragma once
#include "TileEntityBase.hpp"
#include <glm/vec3.hpp>

class EnemySpaceShipTile : TileEntityBase
{
private:
	ModelPosition m_shipPos;

	enum class MovementPattern {
		Circle, Wave, Straight
	};
	MovementPattern m_movementPattern;

	void moveInPattern(MovementPattern mp);
public:
	EnemySpaceShipTile(Game* const ref_game, int seed);

	void update(double deltaTime);
	void draw();

	const ModelPosition& getSpaceshipPos();
};