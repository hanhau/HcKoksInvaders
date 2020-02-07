#include "../include/HighscoreManager.hpp"
#include <string>
#include <fstream>
#include <filesystem>

const std::string HighscoreManager::savePath = "highscore.lbs";

void HighscoreManager::updateEntry(const int points, const int stage)
{
	int currentPoints = 0;
	int currentStage = 0;

	get(currentPoints, currentStage);

	// only update if points are higher than last
	if (points > currentPoints) {
		std::fstream file(savePath, std::ios::out | std::ios::trunc | std::ios::binary);
		
		file.write((char*)&points,sizeof(int));
		file.write((char*)&stage, sizeof(int));
	}
}

void HighscoreManager::get(int& points, int& stage) 
{
	if (!std::filesystem::exists(savePath)) {
		points = stage = 0;
		return;
	}

	std::ifstream file(savePath, std::ios::in | std::ios::binary);

	char bufPoints[4];
	char bufStage[4];

	file.read(bufPoints, 4);
	file.read(bufStage, 4);

	points = *reinterpret_cast<int*>(bufPoints);
	stage = *reinterpret_cast<int*>(bufStage);
}