#pragma once
#include "ModelPosition.hpp"
#include <vector>
#include <glm/mat4x4.hpp>

class InstanceBuffer {
	unsigned int gl_ssbo;
	
	std::vector<ModelPosition> m_positions;
	std::vector<glm::mat4> m_transforms;
	
	const int m_capacity;
	int m_innerCount;
public:
	InstanceBuffer(const int capacity);
	~InstanceBuffer();

	void bind(int index) const;
	void unbind() const;

	void transferToGpu();

	int getCapacity() const;
	void setInnerCount(int count);
	int getInnerCount() const;

	ModelPosition& operator[](unsigned int pos);
};