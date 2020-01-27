#pragma once
#include <string>

struct NetworkManagerResult {
	bool success;
	std::string errMessage;

	inline NetworkManagerResult() :
		success(true), errMessage("") 
	{}
};

struct UserLoginValidResult : NetworkManagerResult {
	bool valid;
};

struct UserIDResult : NetworkManagerResult {
	int userid;
};

struct UploadHighscoreResult : NetworkManagerResult {
};

struct UserStatisticsResult : NetworkManagerResult {
	int played_games;
	int highscorePoints;
	int highscoreStages;
};

struct VersionCurrentResult : NetworkManagerResult {
	bool isCurrent;
};
