#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

struct Bullet {
	enum Owner {
		Enemy,
		Player
	};

	glm::vec3 m_pos;
	glm::vec3 m_speed;
	float m_damage;
	Owner m_owner;

	inline void update(float deltaTime) {
		m_pos += (m_speed * deltaTime);
	}

	inline glm::vec3&& orientation() {
		return glm::normalize(m_speed);
	}
};