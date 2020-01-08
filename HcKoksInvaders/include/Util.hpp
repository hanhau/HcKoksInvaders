#pragma once
#include <corecrt_memory.h>
#include <string>
#include <iostream>
#include <SFML/Window/Window.hpp>

namespace util {
	inline void checkGlCalls(const std::string func_name) {
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