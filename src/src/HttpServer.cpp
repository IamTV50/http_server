#include <string>
#include <vector>
#include <thread>
#include "../include/HttpServer.h"
#include "../include/ErrorMessageException.h"
#include "../include/Parser.h"
#include "../include/PageReader.h"

HttpServer::~HttpServer() {
    WSACleanup();
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

SOCKET HttpServer::initializeListenSocket() {
    int result = 0;
    SOCKET tmpListenSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(tmpListenSoc == INVALID_SOCKET) {
        throw ErrorMessageException("socket function failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Set SO_REUSEADDR option
    int opt = 1;
    if (setsockopt(tmpListenSoc, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        closesocket(tmpListenSoc);
        throw ErrorMessageException("setsockopt failed with error: " + std::to_string(WSAGetLastError()));
    }

    //bind tmpListenSoc to serverAddress
    result = bind(tmpListenSoc, (SOCKADDR *)  &serverAddress, sizeof(serverAddress));
    if(result == SOCKET_ERROR){
        throw ErrorMessageException("bind function failed with error: " + std::to_string(WSAGetLastError()));
    }

    return tmpListenSoc;
}

SOCKET HttpServer::acceptClientSocket(SOCKET listenSoc) {
    SOCKET tmpClientSoc = accept(listenSoc, nullptr, nullptr);
    if(tmpClientSoc == INVALID_SOCKET){
        throw ErrorMessageException("accept function failed with error: " + std::to_string(WSAGetLastError()));
    }

    return tmpClientSoc;
}

void HttpServer::sendResponse(SOCKET clientSocket, const std::string& response) {
    int result = 0;

    result = send(clientSocket, response.c_str(), response.length(), 0);
    if(result == SOCKET_ERROR){
        throw ErrorMessageException("send function failed with error: " + std::to_string(WSAGetLastError()));
    }
}

void HttpServer::listenForConnection() {
    SOCKET listenSocket = initializeListenSocket();

    //listen for incoming connection requests on created socket
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        throw ErrorMessageException("listen function failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Accept incoming connections and add them to the queue
    Parser parser;
    HttpRequest request;
    char recvBuff[1024] = {0};

    SOCKET clientSocket = acceptClientSocket(listenSocket);
    recv(clientSocket, recvBuff, sizeof(recvBuff), 0);

    std::string buffStr(recvBuff);
    request = parser.parseRequest(&buffStr);

    request.clientSocket = clientSocket;
    request.listenSocket = listenSocket;

    {
        std::lock_guard<std::mutex> lock(requestsMutex);
        requests.push(request);
    } //auto unlock mutex
}

void HttpServer::continuouslyListen() {
    std::vector<std::thread> listenerThreads;

    //create initial number of listening threads
    for(int i = 0; i < listenThreadsNum; i++){
        listenerThreads.emplace_back(std::thread(&HttpServer::listenForConnection, this));
    }

    std::mutex listenThreadsMutex;
    while(true){
        for(auto it = listenerThreads.begin(); it != listenerThreads.end(); ){
            if(it->joinable()){
                std::lock_guard<std::mutex> lock(listenThreadsMutex);
                it->join();
                it = listenerThreads.erase(it);
            }
            else{ it++; }
        }

        while(listenerThreads.size() < this->listenThreadsNum){
            std::lock_guard<std::mutex> lock(listenThreadsMutex);
            listenerThreads.emplace_back(std::thread(&HttpServer::listenForConnection, this));
        }
    }
}

void HttpServer::handleRequest(HttpRequest request) {
    PageReader reader;
    std::string httpResponse = "";
    auto connectionHeaderIt = request.headers.find("Connection");

    if(connectionHeaderIt != request.headers.end() && connectionHeaderIt->second == "keep-alive") {
        std::string htmlPageStr = reader.readPage(request.reqUrl);

        if(htmlPageStr == "500"){
            std::string serverErr = "500 Internal Server Error";

            httpResponse = "HTTP/1.1 500 Internal Server Error\n";
            httpResponse += "Content-Length: " + std::to_string(serverErr.length()) + "\n";
            httpResponse += "Connection: Keep-Alive\n";
            httpResponse += "Content-Type: text/html\n";
            httpResponse += "\n";
            httpResponse += serverErr;
        }
        else{
            httpResponse = "HTTP/1.1 200 OK\n";
            httpResponse += "Content-Length: " + std::to_string(htmlPageStr.length()) + "\n";
            httpResponse += "Connection: Keep-Alive\n";
            httpResponse += "Content-Type: text/html\n";
            httpResponse += "\n";
            httpResponse += htmlPageStr;
        }
    }
    else{
        httpResponse = "HTTP/1.1 200 OK\n";
        httpResponse += "Connection: Close\n";
        httpResponse += "\n";
    }

    sendResponse(request.clientSocket, httpResponse);
    request.empty();
}

void HttpServer::run() {
    //continuously listen for incoming connections
    std::thread listening(&HttpServer::continuouslyListen, this);
    listening.detach();

    std::cout<<"server running on: http://"<<this->host<<":"<<this->port<<std::endl;

    std::vector<std::thread> clientHandlingThreads;
    while(true){
        //if(!requests.empty() && clientHandlingThreads.size() < clientHandleThreadsNum){
        while(!requests.empty() && clientHandlingThreads.size() < clientHandleThreadsNum){
            std::lock_guard<std::mutex> lock(requestsMutex);
            clientHandlingThreads.emplace_back(std::thread(&HttpServer::handleRequest, this, requests.front()));
            requests.pop();
        }

        for(auto it = clientHandlingThreads.begin(); it != clientHandlingThreads.end(); ) {
            if (it->joinable()) {
                it->join();
                it = clientHandlingThreads.erase(it);
            } else { it++; }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
