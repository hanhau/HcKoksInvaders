#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <Camera.hpp>
#include <ModelPosition.hpp>

class BoundingBall {
private:
	glm::vec3 m_pos;
	float m_radius;
public:
	BoundingBall();
	BoundingBall(const glm::vec3 pos,const float radius);

	bool intersects(const BoundingBall& b) const;
	const BoundingBall&& transform(const glm::mat4& mat) const;
	const BoundingBall&& transform(const ModelPosition& modelPos) const;

	bool isInViewport(const Camera& cam) const;

	const glm::vec3& getPos() const;
	const float& getRadius() const;
};