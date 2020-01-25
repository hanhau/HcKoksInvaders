#include "include/Texture.hpp"
#include <gl/glew.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

bool Texture::loadFromMemory(uint8_t* const bufferFile, size_t bufferLength) {
	unsigned char* temp = stbi_load_from_memory(
		bufferFile, bufferLength, 
		&m_size.x, &m_size.y, &m_nrChannels, 
		0
	);

	m_bufferLength = m_size.x * m_size.y * m_nrChannels;
	m_buffer = std::unique_ptr<uint8_t>(new uint8_t[m_bufferLength]);

	memcpy_s(m_buffer.get(), m_bufferLength, temp, m_bufferLength);
	
	stbi_image_free(temp);
	return true;
}
void Texture::cleanBuffer() {
	m_buffer.release();
	m_bufferLength = 0;
}

void Texture::uploadToGl() {
	try {
		GLint format = getFormat();

		glGenTextures(1, &gl_textureID);
		glBindTexture(GL_TEXTURE_2D, gl_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D, 0, format,
			m_size.x, m_size.y, 0, format,
			GL_UNSIGNED_BYTE, (void*)m_buffer.get()
		);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	catch (std::string e) {
		std::cout << e << "\n";
	}
}

void Texture::cleanFromGl() {
	glDeleteTextures(1, &gl_textureID);
}

unsigned int Texture::getGlID() const {
	return gl_textureID;
}
glm::ivec2 Texture::getSize() const {
	return m_size;
}
uint8_t* const Texture::getBuffer() const {
	return m_buffer.get();
}
const int Texture::getFormat() const {
	switch (m_nrChannels) {
	case 1: case 2: default:
		throw std::exception(std::string("Not supported Format of Img @ " + std::string(__FUNCTION__)).c_str());
		break;
	case 3: return GL_RGB; break;
	case 4: return GL_RGBA; break;
	}
}