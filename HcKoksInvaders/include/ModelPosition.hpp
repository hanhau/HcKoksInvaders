#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/System/Vector3.hpp>

class ModelPosition {
	glm::mat4x4 m_translation;
	glm::mat4x4 m_rotation;
	glm::mat4x4 m_scale;

	glm::mat4x4 m_transform;
	inline void calcTransform() {
		m_transform = std::move(m_translation * m_scale * m_rotation);
	}
public:
	inline ModelPosition() :
		m_translation(glm::identity<glm::mat4x4>()),
		m_rotation(glm::identity<glm::mat4x4>()),
		m_scale(glm::identity<glm::mat4x4>()),
		m_transform(glm::identity<glm::mat4x4>())
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
	inline ModelPosition(
		glm::vec3 pos,
		float deg, glm::vec3 rot,
		glm::vec3 scale)
	{
		m_translation = glm::translate(
			glm::identity<glm::mat4x4>(),
			pos
		);
		m_rotation = glm::rotate(
			glm::identity<glm::mat4x4>(),
			deg, rot
		);
		m_scale = glm::scale(
			glm::identity<glm::mat4x4>(),
			scale
		);

		calcTransform();
	}

	inline void setTranslationMatrix(sf::Vector3f translation) {
		m_translation = glm::translate(
			glm::identity<glm::mat4x4>(),
			glm::vec3(translation.x, translation.y, translation.z)
		);

		calcTransform();
	}
	inline void setRotationMatrix(float deg, sf::Vector3f weight) {
		m_rotation = glm::rotate(
			glm::identity<glm::mat4x4>(),
			deg, glm::vec3(weight.x, weight.y, weight.z)
		);

		calcTransform();
	}
	inline void setScalingMatrix(sf::Vector3f scale) {
		m_scale = glm::scale(
			glm::identity<glm::mat4x4>(),
			glm::vec3(scale.x, scale.y, scale.z)
		);

		calcTransform();
	}

	inline const glm::mat4x4& getTranslationMatrix() const {
		return m_translation;
	}
	inline const glm::mat4x4& getRotationMatrix() const {
		return m_rotation;
	}
	inline const glm::mat4x4& getScalingMatrix() const {
		return m_scale;
	}

	inline const glm::mat4x4& getTransformationMatrix() const {
		return m_transform;
	}
};