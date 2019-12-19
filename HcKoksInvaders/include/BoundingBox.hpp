#pragma once
#include <SFML\System\Vector3.hpp>
#include "Model.hpp"
#include <vector>

class BoundingBox {
	float m_vals[6];
public:
	BoundingBox();

	void create(std::vector<Vertex>& const vertices);
	bool isVisible(glm::mat4& mat);
};