#include "include/TileEntityBase.hpp"

TileEntityBase::TileEntityBase(Game* const game) :
	m_gameRef(game)
{}
TileEntityBase::TileEntityBase() : m_gameRef(nullptr) {}

void TileEntityBase::update(double deltaTime) {};
void TileEntityBase::draw() {};

void TileEntityBase::letShoot(const glm::vec3 pos, const glm::vec2 orientation) {};

const glm::vec3& TileEntityBase::getPos() const {
	static glm::vec3 _temp;
	return _temp;
};

TileEntityBase::~TileEntityBase() {};