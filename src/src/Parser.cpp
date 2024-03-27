#include "../include/Parser.h"
#include <string>
#include <sstream>

HttpRequest Parser::parseRequest(const std::string *requestStr) {
    HttpRequest paredData;

    //check if "requestStr" is empty
    if(requestStr->empty()){ return  paredData; }

    std::string tmpReqLine = "";
    std::stringstream requestStrStream(*requestStr);

    //get and seperate/extract request method and url
    std::getline(requestStrStream, tmpReqLine, '\n');

    std::stringstream methodAndUrlStream(tmpReqLine);
    methodAndUrlStream >> paredData.reqMethod >> paredData.reqUrl;

    //get headers
    while(std::getline(requestStrStream, tmpReqLine, '\n')){
        if(tmpReqLine == "\r"){ break; } // empty line seperates headers from request body

        std::stringstream tmpHeaderStream(tmpReqLine);
        std::string headerKey, headerValue;

        tmpHeaderStream >> headerKey >> headerValue;
        headerKey.pop_back(); //removes ":" from the end of headerKey

        paredData.headers.emplace(headerKey, headerValue);
    }

    //get the body
    while(std::getline(requestStrStream, tmpReqLine)){
        paredData.body += tmpReqLine;
    }

    return paredData;
}
