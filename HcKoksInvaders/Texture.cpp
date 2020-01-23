#include "include/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

bool Texture::loadFromMemory(uint8_t* const bufferFile, size_t bufferLength) {
	m_buffer = std::make_unique<uint8_t>(stbi_load_from_memory(bufferFile, bufferLength, &m_size.x, &m_size.y, &m_nrChannels, 0));	
	return true;
}

void Texture::uploadToGl() {

}