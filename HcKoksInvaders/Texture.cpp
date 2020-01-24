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
		0 //STBI_rgb_alpha
	);

	m_buffer = std::unique_ptr<uint8_t>(new uint8_t[m_size.x * m_size.y]);
	m_bufferLength = bufferLength;
	
	stbi_image_free(temp);
	return true;
}

void Texture::uploadToGl() {
	try {
		GLint format;
		switch (m_nrChannels) {
		case 1: case 2: default:
			throw "Not supported Format of Img @ " + std::string(__FUNCSIG__);
			break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}

		glGenTextures(1, &gl_textureID);
		glBindTexture(GL_TEXTURE_2D, gl_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D, 0, format,
			m_size.x, m_size.y, 0, format,
			GL_UNSIGNED_BYTE, nullptr//m_buffer.get()
		);
		glGenerateMipmap(GL_TEXTURE_2D);

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