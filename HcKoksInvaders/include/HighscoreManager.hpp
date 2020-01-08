#pragma once
#include <string>

class HighscoreManager {
	static const std::string savePath;
public:
	static void updateEntry(const int points, const int stage);
	static void get(int& points, int& stage);
};