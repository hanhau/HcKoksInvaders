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
	static std::string getDataString(const std::string valName,
									 const std::string input);
public:
	// basic functionality
	static void init(const std::string ip, const std::string host);

	static bool sendHttpsRequest(const RequestType type,
								 const std::string url,
								 std::string& response);

	static void shutdown();

	// specialised functions
	static std::future<UserLoginValidResult> 
		verifyUserLoginValid(const std::string login,
							 const std::string password);

	static std::future<UserIDResult> 
		getUserID(const std::string login,
			      const std::string password);

	static std::future<UploadHighscoreResult> 
		uploadHighscore(const int userID, 
						const int highscore, 
						const int stages);

	static std::future<UserStatisticsResult> 
		getUserStatistics(const int userID);

	static std::future<VersionCurrentResult> 
		checkVersionCurrent(const std::string appVersionString);
};