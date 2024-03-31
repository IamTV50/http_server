#ifndef SRC_HTTPSERVER_H
#define SRC_HTTPSERVER_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class HttpServer {
private:
    std::string host;
    unsigned int port;
    sockaddr_in serverAddress;
    SOCKET listenSocket = INVALID_SOCKET;

    void setServerAddress();
    void initializeListenSocket();

public:
    HttpServer();
    HttpServer(std::string host_, int port_);
    void run();
};

#endif //SRC_HTTPSERVER_H
