#pragma once
#include "TileEntityBase.hpp"
#include <glm/vec3.hpp>

class EnemySpaceShipTile : public TileEntityBase
{
private:
	ModelPosition m_shipPos;
	glm::vec3 m_pos;

	enum class MovementPattern {
		Circle, Wave, Straight
	};
	MovementPattern m_movementPattern;

	//void moveInPattern(MovementPattern mp);
public:
	EnemySpaceShipTile(
		Game* const ref_game, 
		int seed,
		const glm::vec3 pos,
		float scale
	);

	void update(double deltaTime);
	void draw();

	const ModelPosition& getSpaceshipPos();
	const glm::vec3& getPos();
};