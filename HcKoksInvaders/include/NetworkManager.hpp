#pragma once
#include <string>

enum class RequestType {
	GET,
	POST
};

class NetworkManager {
	struct impl;
	static impl* m_impl;

	static int sendPacket(const char* buf);
	static std::string recvPacket();

	static std::string encodeStringToUrl(std::string input);
	static std::string getDataString(const std::string valName,const std::string input);
public:
	static void init(const std::string ip, const std::string host);

	static bool sendHttpsRequest(const RequestType type,
								 const std::string url,
								 std::string& res);

	static bool verifyUserLoginValid(const std::string login,
									 const std::string password,
									 std::string& errorMessage);

	static bool getUserID(const std::string login,
						  const std::string password,
						  int& userID);

	static bool uploadHighscore(const int userID, 
								const int highscore, 
								const int stages);

	static bool getUserStatistics(const int userID,
								  int& played_games,
								  int& highscorePoints,
								  int& highscoreStages);

	static void shutdown();
};