#pragma once
#include <string>

class NetworkManager {
public:
	static void init();
	static void sendHttpsPOSTRequest(const std::string url);
	static void shutdown();
};