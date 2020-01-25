#include "include/InstanceBuffer.hpp"
#include <GL/glew.h>
#include <string>

InstanceBuffer::InstanceBuffer(const int capacity) : 
	m_capacity(capacity), m_innerCount(capacity)
{
	m_positions.resize(capacity);
	m_transforms.resize(capacity);

	glCreateBuffers(1, &gl_ssbo);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER,gl_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * capacity, (void*)0, GL_STREAM_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

InstanceBuffer::~InstanceBuffer() {
	glDeleteBuffers(1,&gl_ssbo);
}

void InstanceBuffer::bind(int index) const {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, gl_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, gl_ssbo);
}
void InstanceBuffer::unbind() const {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void InstanceBuffer::transferToGpu() {
	for (int i = 0; i < m_innerCount; i++) {
		m_transforms[i] = m_positions[i].getTransformationMatrix();
	}

	glNamedBufferSubData(
		gl_ssbo,
		0,
		sizeof(glm::mat4) * m_innerCount,
		m_transforms.data()
	);
}

int InstanceBuffer::getCapacity() const{
	return m_capacity;
}
void InstanceBuffer::setInnerCount(int count) {
	m_innerCount = count;
}
int InstanceBuffer::getInnerCount() const {
	return m_innerCount;
}


ModelPosition& InstanceBuffer::operator[](unsigned int pos) {
	if (pos > m_capacity - 1)
		throw "Out of Bounds @ " + std::string(__FUNCTION__);

	return m_positions[pos];
}