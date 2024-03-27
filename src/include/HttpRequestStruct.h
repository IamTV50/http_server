#ifndef SRC_HTTPREQUESTSTRUCT_H
#define SRC_HTTPREQUESTSTRUCT_H

#include <string>
#include <map>

struct HttpRequest {
    std::string reqMethod = ""; // GET, POST, ...
    std::string reqUrl = ""; // e.g. /home
    std::map<std::string, std::string> headers;  // e.g {"Content-Type" : "text/plain"} and others
    std::string body = ""; // empty by default

    void empty(){
        reqMethod = "";
        reqUrl = "";
        body = "";
        headers.clear();
    }
};

#endif //SRC_HTTPREQUESTSTRUCT_H
