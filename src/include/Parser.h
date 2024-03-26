#ifndef SRC_PARSER_H
#define SRC_PARSER_H

#include <string>
#include "HttpRequestStruct.h"

class Parser {
public:
    Parser() = default;
    HttpRequest parseRequest(const std::string* requestStr);
};


#endif //SRC_PARSER_H
