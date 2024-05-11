#ifndef SRC_HTTPREQUESTSTRUCT_H
#define SRC_HTTPREQUESTSTRUCT_H

#include <string>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>

struct HttpRequest {
    std::string reqMethod = ""; // GET, POST, ...
    std::string reqUrl = ""; // e.g. /home
    std::map<std::string, std::string> headers;  // e.g {"Content-Type" : "text/plain"} and others
    std::string body = ""; // empty by default
    SOCKET clientSocket = INVALID_SOCKET;
    SOCKET listenSocket = INVALID_SOCKET;

    void empty(){
        reqMethod = "";
        reqUrl = "";
        body = "";
        headers.clear();
        closesocket(clientSocket);
        closesocket(listenSocket);
    }
};

#endif //SRC_HTTPREQUESTSTRUCT_H
