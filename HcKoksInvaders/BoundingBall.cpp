#include "include/BoundingBall.hpp"

BoundingBall::BoundingBall() : 
	m_pos(), m_radius(FLT_MIN)
{}
BoundingBall::BoundingBall(const glm::vec3 pos,const float radius)  :
	m_pos(pos), m_radius(radius)
{}

bool BoundingBall::intersects(const BoundingBall& b) const {
	return glm::distance(m_pos, b.getPos()) <= (m_radius + b.getRadius());
}

const BoundingBall&& BoundingBall::transform(const glm::mat4& mat) {
	return std::move(BoundingBall(
		(glm::vec3)(mat * glm::vec4(m_pos, 1.0)),
		(mat * glm::vec4(m_radius)).x
	));
}

const glm::vec3& BoundingBall::getPos() const {
	return m_pos;
}
const float& BoundingBall::getRadius() const {
	return m_radius;
}