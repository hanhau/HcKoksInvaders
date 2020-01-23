#pragma once
#include <glm/vec2.hpp>
#include <memory>

class Texture {
	std::unique_ptr<uint8_t> m_buffer;
	size_t m_bufferLength;

	unsigned int gl_textureID;
	glm::ivec2 m_size;
	int m_nrChannels;
public:
	bool loadFromMemory(uint8_t * const bufferFile,size_t bufferLength);

	void uploadToGl();
	int getGlID() const;
	glm::ivec2 getSize() const;
};