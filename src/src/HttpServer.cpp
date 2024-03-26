#include <string>
#include "../include/HttpServer.h"
#include "../include/ErrorMessageException.h"
#include "../include/Parser.h"

HttpServer::HttpServer() {
    this->host = "127.0.0.1";
    this->port = 8080;

    setServerAddress();
}

HttpServer::HttpServer(std::string host_, int port_): host(host_), port(port_) {
    setServerAddress();
}

void HttpServer::setServerAddress() {
    WSADATA wsaData = {0};
    WSAStartup(MAKEWORD(2,2), &wsaData);

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = inet_addr(host.c_str());
    this->serverAddress.sin_port = htons(port);
}

void HttpServer::run() {
    int result = 0;

    //create socket for listening
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenSocket == INVALID_SOCKET) {
        throw ErrorMessageException("socket function failed with error: " + std::to_string(WSAGetLastError()));
    }

    //bind listenSocket to serverAddress
    result = bind(listenSocket, (SOCKADDR *)  &serverAddress, sizeof(serverAddress));
    if(result == SOCKET_ERROR){
        throw ErrorMessageException("bind function failed with error: " + std::to_string(WSAGetLastError()));
    }

    //listen for incoming connection requests on created socket
    if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
        throw ErrorMessageException("listen function failed with error: " + std::to_string(WSAGetLastError()));
    }
    std::cout<<"listening on listenSocket..."<<std::endl;

    //create new socket for use of receiving and sending
    SOCKET clientSocket;
    std::cout<<"Waiting for client to connect..."<<std::endl;

    //accepts the connection
    clientSocket = accept(listenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        throw ErrorMessageException("accept function failed with error: " + std::to_string(WSAGetLastError()));
    }

    std::cout<<"client connected..."<<std::endl;
    char recvBuff[1024] = {0};
    bool keepAlive = false;
    Parser parser;
    HttpRequest request;

    do {
        request.empty();

        recv(clientSocket, recvBuff, sizeof(recvBuff), 0);

        std::string str(recvBuff);
        request = parser.parseRequest(&str);

        auto connectionHeaderIt = request.headers.find("Connection");

        if(connectionHeaderIt != request.headers.end() && connectionHeaderIt->second == "keep-alive") {
            keepAlive = true;
        }

        std::string httpResponse = "HTTP/1.1 200 OK\n";
        httpResponse += "Content-Length: 0\n";
        httpResponse += "\n";

        result = send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
        if(result == SOCKET_ERROR){
            keepAlive = false;
            throw ErrorMessageException("send function failed with error: " + std::to_string(WSAGetLastError()));
        }
    }while(keepAlive);
}