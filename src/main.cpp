#include <iostream>
#include <string>
#include "./include/HttpServer.h"

int main() {
    HttpServer server("127.0.0.1", 9999);

    try{
        server.run();
    }
    catch(std::exception err){
        err.what();
    }

    return 0;
}
