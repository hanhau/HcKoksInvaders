#pragma once
#include "Game.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class TileEntityBase {
protected:
	Game * const m_gameRef;
public:
	TileEntityBase(Game* const game);
	TileEntityBase();

	virtual void update(double deltaTime);
	virtual void draw();

	virtual void letShoot(const glm::vec3 pos,const glm::vec2 orientation);
	virtual const glm::vec3& getPos() const;
	
	virtual ~TileEntityBase();
};