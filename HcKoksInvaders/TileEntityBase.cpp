#include "include/TileEntityBase.hpp"

TileEntityBase::TileEntityBase(Game* const game) :
	m_gameRef(game)
{}
TileEntityBase::TileEntityBase() : m_gameRef(nullptr) {}

void TileEntityBase::update(double deltaTime) {};
void TileEntityBase::draw() {};
TileEntityBase::~TileEntityBase() {};