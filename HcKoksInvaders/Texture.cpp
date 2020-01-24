#include "include/Texture.hpp"
#include <gl/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

bool Texture::loadFromMemory(uint8_t* const bufferFile, size_t bufferLength) {
	unsigned char* temp = stbi_load_from_memory(
		bufferFile, bufferLength, 
		&m_size.x, &m_size.y, &m_nrChannels, 
		STBI_rgb_alpha
	);

	m_buffer = std::make_unique<uint8_t>(new uint8_t[m_size.x * m_size.y]);
	m_bufferLength = bufferLength;
	
	stbi_image_free(temp);
	return true;
}

void Texture::uploadToGl() {
	glGenTextures(1, &gl_textureID);
	glBindTexture(GL_TEXTURE_2D,gl_textureID);

	glTexImage2D(GL_TEXTURE_2D,0,)
}

unsigned int Texture::getGlID() const {
	return gl_textureID;
}
glm::ivec2 Texture::getSize() const {
	return m_size;
}