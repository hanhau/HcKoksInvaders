#include "include/BoundingBox.hpp"
#include <glm/ext/matrix_relational.hpp>

enum bb_val {
	X_MIN = 0,
	X_MAX = 1,
	Y_MIN = 2,
	Y_MAX = 3,
	Z_MIN = 4,
	Z_MAX = 5
};

BoundingBox::BoundingBox() {
	for (int i = 0; i < 6; i++)
		m_vals[i] = 0.0f;
}

void BoundingBox::create(std::vector<Vertex>& const vertices) {
	for (auto& const iter : vertices) {
		if (iter.m_position.x < m_vals[X_MIN])
			m_vals[X_MIN] = iter.m_position.x;
		if (iter.m_position.x > m_vals[X_MAX])
			m_vals[X_MAX] = iter.m_position.x;
		
		if (iter.m_position.y < m_vals[Y_MIN])
			m_vals[Y_MIN] = iter.m_position.y;
		if (iter.m_position.y > m_vals[Y_MAX])
			m_vals[Y_MAX] = iter.m_position.y;
		
		if (iter.m_position.z < m_vals[Z_MIN])
			m_vals[Z_MIN] = iter.m_position.z;
		if (iter.m_position.z > m_vals[Z_MAX])
			m_vals[Z_MAX] = iter.m_position.z;
	}
}

bool BoundingBox::isVisible(glm::mat4& mat) {
	glm::vec3 points[8] = {
		// front
		glm::vec3(m_vals[X_MIN],m_vals[Y_MAX],m_vals[Z_MAX]),
		glm::vec3(m_vals[X_MAX],m_vals[Y_MAX],m_vals[Z_MAX]),
		glm::vec3(m_vals[X_MAX],m_vals[Y_MIN],m_vals[Z_MAX]),
		glm::vec3(m_vals[X_MIN],m_vals[Y_MIN],m_vals[Z_MAX]),
		
		// back
		glm::vec3(m_vals[X_MIN],m_vals[Y_MAX],m_vals[Z_MIN]),
		glm::vec3(m_vals[X_MAX],m_vals[Y_MAX],m_vals[Z_MIN]),
		glm::vec3(m_vals[X_MAX],m_vals[Y_MIN],m_vals[Z_MIN]),
		glm::vec3(m_vals[X_MIN],m_vals[Y_MIN],m_vals[Z_MIN])
	};

	for (int i = 0; i < 8; i++) {
		glm::vec4 pt(mat * glm::vec4(points[i],1.0f));

		if (pt.x >= -1.0f && pt.x <= 1.0f &&
			pt.y >= -1.0f && pt.y <= 1.0f)
			return true;
	}

	return false;
}