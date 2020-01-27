#pragma once
#include "NetworkManagerResult.hpp"
#include <string>
#include <future>

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
								 std::string& response);

	static std::future<UserLoginValidResult> 
		verifyUserLoginValid(const std::string login,
							 const std::string password);

	static std::future<UserIDResult> 
		getUserID(const std::string login,
			      const std::string password);

	static bool uploadHighscore(const int userID, 
								const int highscore, 
								const int stages);

	static bool getUserStatistics(const int userID,
								  int& played_games,
								  int& highscorePoints,
								  int& highscoreStages);

	static std::future<VersionCurrentResult> 
		checkVersionCurrent(const std::string appVersionString,
							bool& result);

	static void shutdown();
};