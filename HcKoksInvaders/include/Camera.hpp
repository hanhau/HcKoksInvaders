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
	Camera();
	Camera(
		const glm::vec3 pos,
		const glm::vec3 front,
		const glm::vec3 up,
		const glm::mat4 projectionMatrix
	);

	void setCameraPos(const glm::vec3 pos);
	void setCameraFront(const glm::vec3 front);
	void setCameraUp(const glm::vec3 up);

	void setProjectionMatrix(const glm::mat4 proj);

	const glm::vec3& getCameraPos() const;
	const glm::mat4& getProjectionMatrix() const;
	const glm::mat4&& getViewMatrix() const;
};