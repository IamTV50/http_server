#ifndef SRC_HTTPREQUESTSTRUCT_H
#define SRC_HTTPREQUESTSTRUCT_H

#include <string>
#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

struct HttpRequest {
    std::string reqMethod = ""; // GET, POST, ...
    std::string reqUrl = ""; // e.g. /home
    std::map<std::string, std::string> headers;  // e.g {"Content-Type" : "text/plain"} and others
    std::string body = ""; // empty by default
    SOCKET clientSocket = INVALID_SOCKET;
    SOCKET listenSocket = INVALID_SOCKET;
    std::string imageExtension = "";

    std::string checkForExenesion(std::string path){
        std::stringstream pathStream(path);
        std::string tmpRead;
        std::string extension = "";
        int i = 0;

        while(getline(pathStream, tmpRead, '.')){
            extension = tmpRead;
            i++;
        }

        // If more than one segment was found, the path has an extension
        if(i > 1){ return extension; }
        else{ return ""; }
    }

    void empty(){
        reqMethod = "";
        reqUrl = "";
        imageExtension = "";
        body = "";
        headers.clear();
        closesocket(clientSocket);
        closesocket(listenSocket);
    }
};

#endif //SRC_HTTPREQUESTSTRUCT_H
