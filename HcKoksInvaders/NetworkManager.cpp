#include "include/NetworkManager.hpp"

#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <ws2tcpip.h>
#include <rpc.h>
#include <ntdsapi.h>
#include <stdio.h>
#include <tchar.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <sstream>
#include <filesystem>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "fwpuclnt.lib")
#pragma comment(lib, "ntdsapi.lib")

struct NetworkManager::impl {
    SSL* ssl;
    int sock;
    WSADATA* wsadata;

    std::string host;
    std::string ip;
};
NetworkManager::impl* NetworkManager::m_impl;

std::string NetworkManager::recvPacket()
{
    std::string res;

    int bufLen = 1000;
    size_t readBytes = 0;
    char * buf = new char[bufLen+1];
    
    do {
        readBytes = SSL_read(m_impl->ssl, buf, bufLen);
        buf[bufLen] = 0;
        res.append(buf, readBytes);
    } while (readBytes > 0);

    if (readBytes < 0) {
        int err = SSL_get_error(m_impl->ssl, bufLen);
        return "SSL Error " + std::to_string(err);
    }

    return res;
}
int NetworkManager::sendPacket(const char* buf)
{
    int len = SSL_write(m_impl->ssl, buf, strlen(buf));
    if (len < 0) {
        int err = SSL_get_error(m_impl->ssl, len);
        switch (err) {
        case SSL_ERROR_WANT_WRITE:
            return 0;
        case SSL_ERROR_WANT_READ:
            return 0;
        case SSL_ERROR_ZERO_RETURN:
        case SSL_ERROR_SYSCALL:
        case SSL_ERROR_SSL:
        default:
            return -1;
        }
    }
}

std::string NetworkManager::encodeStringToUrl(std::string input) 
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = input.begin(), n = input.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (std::isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char)c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}
std::string NetworkManager::getDataString(const std::string valName, 
                                          const std::string input) 
{
    size_t posBegin = input.find(valName+"({") + valName.length() + strlen("({");
    if (posBegin == std::string::npos)
        return "";

    size_t posEnd = input.find_first_of("})", posBegin);
    if (posEnd == std::string::npos)
        return "";

    auto a = std::string(input.begin() + posBegin, input.begin() + posEnd);
    return std::string(input.begin() + posBegin, input.begin() + posEnd);
}

void NetworkManager::init(const std::string ip, 
                          const std::string host) 
{
    m_impl = new impl;
    m_impl->host = host;
    m_impl->ip = ip;
    m_impl->wsadata = new WSADATA();

    if (WSAStartup(0x0101, m_impl->wsadata))
        std::cout << __FUNCSIG__ << " ERROR\n";
}

void NetworkManager::shutdown() {
    if (WSACleanup())
        std::cout << __FUNCSIG__ << " ERROR\n";
}

bool NetworkManager::sendHttpsRequest(const RequestType type,
                                      const std::string url,
                                      std::string& res)
{
    int s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (!s) {
        printf("Error creating socket.\n");
        return false;
    }
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, m_impl->ip.c_str(), &(sa.sin_addr));

    sa.sin_port = htons(443);
    socklen_t socklen = sizeof(sa);
    if (connect(s, (struct sockaddr*) & sa, socklen)) {
        printf("Error connecting to server.\n");
        return false;
    }
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD* meth = TLSv1_2_client_method();
    SSL_CTX* ctx = SSL_CTX_new(meth);
    m_impl->ssl = SSL_new(ctx);
    if (!m_impl->ssl) {
        printf("Error creating SSL.\n");
        return false;
    }
    m_impl->sock = SSL_get_fd(m_impl->ssl);
    SSL_set_fd(m_impl->ssl, s);
    int err = SSL_connect(m_impl->ssl);
    if (err <= 0) {
        printf("Error creating SSL connection.  err=%x\n", err);
        return false;
    }
    printf("SSL connection using %s\n", SSL_get_cipher(m_impl->ssl));

    std::string request = "";
    request += type == RequestType::GET ? "GET " : "POST ";
    request += url + " HTTP/1.1\r\n";
    request += "Host: "+m_impl->host+"\r\n\r\n";

    sendPacket(request.c_str());
    res = recvPacket();
}

bool NetworkManager::verifyUserLoginValid(const std::string login,
                                          const std::string password,
                                          std::string& errorMessage) 
{
    std::string res;
    if (!sendHttpsRequest(
        RequestType::GET,
        "/verifyUserLoginValid.php?login=" + encodeStringToUrl(login) +
        "&password=" + encodeStringToUrl(password),
        res
    )) {
        errorMessage = "Kann sich nicht mit dem Server verbinden.";
        return false;
    };

    if (res.find("RESULT_OK") != std::string::npos) {
        errorMessage = "";
        return true;
    }
    else if (res.find("RESULT_NOTFOUND") != std::string::npos) {
        errorMessage = "Der Nutzer wurde noch nicht angelegt.";
    }
    else if (res.find("RESULT_INVALIDPASSWORD") != std::string::npos) {
        errorMessage = "Ungültiges Passwort";
    }
    else {
        errorMessage = "Unbekannter Fehler";
    }

    return false;
}

bool NetworkManager::getUserID(const std::string login,
                               const std::string password,
                               int& userID) 
{
    std::string res;
    if (!sendHttpsRequest(
        RequestType::GET,
        "/getUserID.php?login=" + encodeStringToUrl(login) +
        "&password=" + encodeStringToUrl(password),
        res
    )) {
        userID = 0;
        return false;
    };

    if (res.find("RESULT_ERROR") != std::string::npos) {
        userID = 0;
        return false;
    }
    else {
        userID = std::atoi(getDataString("USERID", res).c_str());
        return true;
    }
}

bool NetworkManager::uploadHighscore(const int userID,
                                     const int highscore,
                                     const int stages)
{
    std::string res;
    if (!sendHttpsRequest(
        RequestType::POST,
        "uploadHighscore.php?userid=" + encodeStringToUrl(std::to_string(userID)) +
        "&highscore=" + encodeStringToUrl(std::to_string(highscore)) +
        "&stages=" + encodeStringToUrl(std::to_string(stages)),
        res
    )) {
        return false;
    };

    return true;
}

bool NetworkManager::getUserStatistics(const int userID,
                                       int& played_games,
                                       int& highscorePoints,
                                       int& highscoreStages)
{
    std::string res;
    if (!sendHttpsRequest(
        RequestType::GET,
        "getUserStatistics.php?userid=" + encodeStringToUrl(std::to_string(userID)),
        res
    )) {
        return false;
    };

    played_games = 0;
    highscorePoints = 0;
    highscoreStages = 0;

    std::string playedGamesString = getDataString("PLAYEDGAMES", res);
    std::string highscorePointsString = getDataString("HIGHSCOREPOINTS", res);
    std::string highscoreStagesString = getDataString("HIGHSCORESTAGES", res);

    if (res.find("RESULT_ERROR") != std::string::npos ||
        playedGamesString == "" ||
        highscorePointsString == "" ||
        highscoreStagesString == "") 
    {
        return false;
    }
    
    int games, points, stages;

    try {
        games = std::stoi(playedGamesString);
        points = std::stoi(highscorePointsString);
        stages = std::stoi(highscoreStagesString);
    }
    catch (std::exception e) {
        return false;
    }

    played_games = games;
    highscorePoints = points;
    highscoreStages = stages;

    return true;
}

