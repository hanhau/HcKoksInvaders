#pragma once
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#elif defined __linux__

#endif

class _Log {
	enum class Color {
		ORANGE,RED,GREEN,BLUE,WHITE
	};

#ifdef _WIN32
	void setConsoleColor(Color color) {
		int flag = 0;
		switch (color)
		{
		case _Log::Color::ORANGE:
			flag = 0b0110;
			break;
		case _Log::Color::RED:
			flag = 0b0100;
			break;
		case _Log::Color::GREEN:
			flag = 0b0010;
			break;
		case _Log::Color::BLUE:
			flag = 0b0001;
			break;
		case _Log::Color::WHITE:
			flag = 0b0111;
		default:
			break;
		}

		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout, flag);
	}
#elif defined __linux__
	void setConsoleColor(Color color) {

	}
#endif

	static inline void print(const char* str) {
		std::cout << str;
	}
	template<typename... Args, typename T>
	static void print(const char* str, T value, Args... args) {
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
	template<typename... Args> void debug(const char* line, Args... args) {
		setConsoleColor(Color.ORANGE);
		print("[DEBUG]");
		setConsoleColor(Color.WHITE);
		print(line, args);
		print("\n");
	}
	template<typename... Args> void info(const char* line, Args... args) {
		setConsoleColor(Color.GREEN);
		print("[INFO]");
		setConsoleColor(Color.WHITE);
		print(line, args);
		print("\n");
	}
	template<typename... Args> void error(const char* line, Args... args) {
		setConsoleColor(Color.RED);
		print("[ERROR]");
		setConsoleColor(Color.WHITE);
		print(line, args);
		print("\n");
	}
	template<typename... Args> void opengl(const char* line, Args... args) {
		setConsoleColor(Color.BLUE);
		print("[ERROR]");
		setConsoleColor(Color.WHITE);
		print(line, args);
		print("\n");
	}
	template<typename... Args> void vulkan(const char* line, Args... args) {
		setConsoleColor(Color.BLUE);
		print("[ERROR]");
		setConsoleColor(Color.WHITE);
		print(line, args);
		print("\n");
	}
};

extern _Log Log;