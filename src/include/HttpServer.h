#ifndef SRC_HTTPSERVER_H
#define SRC_HTTPSERVER_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <queue>
#include <mutex>
#include "HttpRequestStruct.h"

class HttpServer {
private:
    std::string host;
    unsigned int port;
    sockaddr_in serverAddress;
    int listenThreadsNum = 2;
    int clientHandleThreadsNum = 4;
    std::queue<HttpRequest> requests;
    std::mutex requestsMutex;

    void setServerAddress();
    SOCKET initializeListenSocket();
    SOCKET acceptClientSocket(SOCKET listenSoc);
    void sendResponse(SOCKET clientSocket, const std::string& response);
    void listenForConnection();
    void continuouslyListen();
    std::string formatResponse(bool keepAlive, int statusCode, std::string contentType, std::string body);
    void handleRequest(HttpRequest request);

public:
    ~HttpServer();
    HttpServer(std::string host_, int port_);
    void run();
};

#endif //SRC_HTTPSERVER_H
