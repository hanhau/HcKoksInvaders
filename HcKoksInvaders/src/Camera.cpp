#include "../include/Camera.hpp"

Camera::Camera() :
	m_projectionMat(glm::identity<glm::mat4>()),
	m_cameraPos(glm::vec3(0.0f)),
	m_cameraFront(glm::vec3(0.0f)),
	m_cameraUp(glm::vec3(0.0f))
{}
Camera::Camera(
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

void Camera::setCameraPos(const glm::vec3 pos) {
	m_cameraPos = pos;
}

void Camera::setCameraFront(const glm::vec3 front) {
	m_cameraFront = front;
}

void Camera::setCameraUp(const glm::vec3 up) {
	m_cameraUp = up;
}

void Camera::setProjectionMatrix(const glm::mat4 proj) {
	m_projectionMat = proj;
}

const glm::vec3& Camera::getCameraPos() const {
	return m_cameraPos;
}

const glm::mat4& Camera::getProjectionMatrix() const {
	return m_projectionMat;
}

const glm::mat4&& Camera::getViewMatrix() const {
	return glm::lookAt(
		m_cameraPos,
		m_cameraPos + m_cameraFront,
		m_cameraUp
	);
}