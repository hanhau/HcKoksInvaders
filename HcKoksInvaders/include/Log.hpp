#pragma once
#include <iostream>

class _Log {
	// Private Functions
	inline void print(const char* str) {
		std::cout << str;
	}
	template<typename... Args, typename T>
	void print(const char* str, T value, Args... args) {
		for (; *str != '\0'; str++) {
			if (*str == '%') {
				std::cout << value;
				print(str + 2, args...);
				return;
			}
			std::cout << *str;
		}
	}
public:
	template<typename... Args>
		void debug(const char* line, Args... args);
	template<typename... Args>
		void info(const char* line, Args... args);
	template<typename... Args>
		void error(const char* line, Args... args);
	template<typename... Args>
		void opengl(const char* line, Args... args);
};

extern _Log Log;