#include <iostream>
#include "./include/HttpServer.h"
#include <string>

using std::cout;
using std::endl;

int main() {
    HttpServer server("127.0.0.1", 9999);

    try{
        server.run();
    }
    catch(std::exception err){
        err.what();
    }

    //todo - if url is "/command/stopserver" run server.stop();

    return 0;
}
