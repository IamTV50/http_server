#include <iostream>
#include <string>
#include "./include/HttpServer.h"

int main() {
    HttpServer server("192.168.1.7", 9999); //use local ipv4 (DON'T USE 127.0.0.1)

    try{
        server.run();
    }
    catch(std::exception err){
        err.what();
    }

    return 0;
}
