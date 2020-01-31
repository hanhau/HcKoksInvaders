#pragma once
#include <initializer_list>
#include <iostream>

enum class LogLocation {
	File, Console
};

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
	void init(std::initializer_list<LogLocation> locations);

	void debug();
	void info();
	void error();
	void opengl();
};

extern _Log Log;