#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/System/Vector3.hpp>

class ModelPosition {
	glm::mat4x4 m_translation;
	glm::mat4x4 m_rotation;
	glm::mat4x4 m_scale;
public:
	inline ModelPosition() :
		m_translation(glm::identity<glm::mat4x4>()),
		m_rotation(glm::identity<glm::mat4x4>()),
		m_scale(glm::identity<glm::mat4x4>())
	{}
	inline ModelPosition(
		sf::Vector3f pos,
		float deg, sf::Vector3f rot,
		sf::Vector3f scale)
	{
		setTranslationMatrix(pos);
		setRotationMatrix(deg, rot);
		setScalingMatrix(scale);
	}

	inline void setTranslationMatrix(sf::Vector3f translation) {
		m_translation = glm::translate(
			glm::identity<glm::mat4x4>(),
			glm::vec3(translation.x, translation.y, translation.z)
		);
	}
	inline void setRotationMatrix(float deg, sf::Vector3f weight) {
		m_rotation = glm::rotate(
			glm::identity<glm::mat4x4>(),
			deg, glm::vec3(weight.x, weight.y, weight.z)
		);
	}
	inline void setScalingMatrix(sf::Vector3f scale) {
		m_scale = glm::scale(
			glm::identity<glm::mat4x4>(),
			glm::vec3(scale.x, scale.y, scale.z)
		);
	}

	inline glm::mat4x4& getTranslationMatrix() {
		return m_translation;
	}
	inline glm::mat4x4& getRotationMatrix() {
		return m_rotation;
	}
	inline glm::mat4x4& getScalingMatrix() {
		return m_scale;
	}

	inline glm::mat4x4&& getTransformationMatrix() {
		return std::move(m_translation * m_scale * m_rotation);
	}
};