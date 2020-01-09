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

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// link with fwpuclnt.lib for Winsock secure socket extensions
#pragma comment(lib, "fwpuclnt.lib")

// link with ntdsapi.lib for DsMakeSpn function
#pragma comment(lib, "ntdsapi.lib")

static WSADATA* wsadata = new WSADATA();

SSL* ssl;
int sock;

int RecvPacket()
{
    int len = 100;
    char * buf = new char[len+1];
    do {
        len = SSL_read(ssl, buf, 100);
        buf[len] = 0;
        printf(buf);
    } while (len > 0);
    if (len < 0) {
        int err = SSL_get_error(ssl, len);
        if (err == SSL_ERROR_WANT_READ)
            return 0;
        if (err == SSL_ERROR_WANT_WRITE)
            return 0;
        if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
            return -1;
    }
}

int SendPacket(const char* buf)
{
    int len = SSL_write(ssl, buf, strlen(buf));
    if (len < 0) {
        int err = SSL_get_error(ssl, len);
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


void NetworkManager::init() {
    if (WSAStartup(0x0101, wsadata))
        return;// std::cout << __FUNCSIG__ << " ERROR\n";
}

void NetworkManager::shutdown() {
    if (WSACleanup())
        return;// std::cout << __FUNCSIG__ << " ERROR\n";

}

void NetworkManager::sendHttpsPOSTRequest(const std::string url)
{
    int s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (!s) {
        printf("Error creating socket.\n");
    }
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, "81.19.159.64", &(sa.sin_addr));

    sa.sin_port = htons(443);
    socklen_t socklen = sizeof(sa);
    if (connect(s, (struct sockaddr*) & sa, socklen)) {
        printf("Error connecting to server.\n");
    }
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD* meth = TLSv1_2_client_method();
    SSL_CTX* ctx = SSL_CTX_new(meth);
    ssl = SSL_new(ctx);
    if (!ssl) {
        printf("Error creating SSL.\n");
    }
    sock = SSL_get_fd(ssl);
    SSL_set_fd(ssl, s);
    int err = SSL_connect(ssl);
    if (err <= 0) {
        printf("Error creating SSL connection.  err=%x\n", err);
        fflush(stdout);
    }
    printf("SSL connection using %s\n", SSL_get_cipher(ssl));

    char* request = (char*)""
        "GET /admin/admin_login.html HTTP/1.1\r\nHost: www.handata.eu\r\n\r\n";
    SendPacket(request);
    RecvPacket();
}
