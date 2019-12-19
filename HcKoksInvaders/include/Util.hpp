#pragma once
#include <corecrt_memory.h>
#include <glm\gtc\type_ptr.hpp>
#include <string>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

namespace util {
	inline void setUniformMat(sf::Shader* shader, std::string name, glm::mat4x4& mat) {
		float tmat[16] = { 0.0f };
		const float* src = (const float*)glm::value_ptr(mat);
		memcpy(tmat, src, 16 * sizeof(float));

		const auto m = sf::Glsl::Mat4(tmat);
		shader->setUniform(name, m);
	}

	inline sf::Vector2f PixelPosToButtonNDC(sf::RenderWindow& rw, sf::Vector2i pos) {
		return sf::Vector2f(
			(pos.x/(float)rw.getSize().x),
			1.0f-(pos.y/(float)rw.getSize().y)
		);
	}

	inline sf::Vector2f PixelLengthToButtonNDC(sf::RenderWindow& rw, sf::Vector2i length) {
		return sf::Vector2f(
			(length.x / (float)rw.getSize().x),
			(length.y / (float)rw.getSize().y)
		);
	}

	inline sf::Vector2f NDCToButtonPixel(sf::RenderWindow& rw, sf::Vector2f pos) {
		return sf::Vector2f(
			pos.x * (float)rw.getSize().x,
			pos.y * (float)rw.getSize().y
		);
	}

	inline void checkGlCalls(const char* func_name) {
		return;
		#ifdef RELEASE_BUILD
			return;
		#else
			GLenum e = glGetError();
			if (e == GL_NO_ERROR)
				return;

			std::cout << func_name << " GL_ErrorCodes: ";
			while (e != GL_NO_ERROR) {
				switch (e) {
					case GL_INVALID_ENUM: std::cout << "InvalidEnum "; break;
					case GL_INVALID_VALUE: std::cout << "InvalidValue "; break;
					case GL_INVALID_OPERATION: std::cout << "InvalidOperation "; break;
					case GL_INVALID_FRAMEBUFFER_OPERATION: std::cout << "InvalidFramebufferOperation "; break;
					case GL_OUT_OF_MEMORY: std::cout << "OutOfMemory "; break;
					case GL_STACK_UNDERFLOW: std::cout << "StackUnderflow "; break;
					case GL_STACK_OVERFLOW: std::cout << "StackOverflow "; break;
				}
				e = glGetError();
			}
			std::cout << "\n";
		#endif
	}
}