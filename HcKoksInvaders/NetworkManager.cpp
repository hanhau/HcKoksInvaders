#include "include/NetworkManager.hpp"

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

namespace {
	static WSADATA wsadata;
}

void NetworkManager::init() {
	if (WSAStartup(0x0101, &wsadata))
		std::cout << __FUNCSIG__ << " ERROR\n";
}

void NetworkManager::shutdown() {
	if (WSACleanup())
		std::cout << __FUNCSIG__ << " ERROR\n";
	
}

void NetworkManager::sendHttpsPOSTRequest(const std::string url)
{

}