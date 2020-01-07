#include "include/BoundingBall.hpp"

BoundingBall::BoundingBall() : 
	m_pos(), m_radius(0.0f)
{}
BoundingBall::BoundingBall(const glm::vec3 pos,const float radius)  :
	m_pos(pos), m_radius(radius)
{}

bool BoundingBall::intersects(const BoundingBall& b) const {
	return glm::distance(m_pos, b.getPos()) <= (m_radius + b.getRadius());
}
bool BoundingBall::intersects(const glm::vec3& point) const {
	return glm::distance(m_pos, point) < m_radius;
}

const BoundingBall&& BoundingBall::transform(const ModelPosition& modelPos) const {
	return transform(modelPos.getTransformationMatrix());
}
const BoundingBall&& BoundingBall::transform(const glm::mat4& mat) const {
	const glm::vec3 res_center = (glm::vec3)(mat * glm::vec4(m_pos,1.0f));
	const glm::vec3 res_centerPR = (glm::vec3)(mat * glm::vec4(m_pos + glm::vec3(m_radius,0.0f,0.0f),1.0f));

	const float transRadius = glm::distance(res_center,res_centerPR);

	return BoundingBall(
		res_center,
		transRadius
	);
}

bool BoundingBall::isInViewport(const Camera& cam) const {
	const BoundingBall tBall = transform(cam.getProjectionMatrix() * cam.getViewMatrix());

	return (
		tBall.getPos().x >= (-1.0f - tBall.getRadius()) &&
		tBall.getPos().x <= ( 1.0f + tBall.getRadius()) &&
		tBall.getPos().y >= (-1.0f - tBall.getRadius()) &&
		tBall.getPos().y <= ( 1.0f + tBall.getRadius())
	);
}

const glm::vec3& BoundingBall::getPos() const {
	return m_pos;
}
const float& BoundingBall::getRadius() const {
	return m_radius;
}