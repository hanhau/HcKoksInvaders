#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;

	glm::mat4 m_projectionMat;
public:
	inline Camera() : m_projectionMat(glm::identity<glm::mat4>()) {}
	inline Camera(
		const glm::vec3 pos, 
		const glm::vec3 front, 
		const glm::vec3 up, 
		const glm::mat4 projectionMatrix) 
	{
		setCameraPos(pos);
		setCameraFront(front);
		setCameraUp(up);
		setProjectionMatrix(projectionMatrix);
	}

	inline void setCameraPos(const glm::vec3 pos) {
		m_cameraPos = pos;
	}

	inline void setCameraFront(const glm::vec3 front) {
		m_cameraFront = front;
	}

	inline void setCameraUp(const glm::vec3 up) {
		m_cameraUp = up;
	}

	inline void setProjectionMatrix(const glm::mat4 proj) {
		m_projectionMat = proj;
	}

	inline const glm::mat4& const getProjectionMatrix() const {
		return m_projectionMat;
	}

	inline const glm::mat4&& getViewMatrix() const {
		return glm::lookAt(
			m_cameraPos, 
			m_cameraPos + m_cameraFront,
			m_cameraUp
		);
	}
};