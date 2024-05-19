#include <string>
#include "../include/PageReader.h"
#include <fstream>

std::string PageReader::read404page(){
    std::string errorPageStr = "";
    std::string path = "../../www/404.html";

    std::ifstream errPage(path);
    if(errPage.is_open()){
        std::string line = "";

        while(std::getline(errPage, line)){
            errorPageStr += line + "\n";
        }
        errPage.close();
    }
    else{
        errorPageStr = "500"; //ugyl but it works
    }

    return errorPageStr;
}

std::string PageReader::readPage(std::string pagePath){
    std::string pageStr = "";

    if(pagePath == "/"){ pagePath = "/index"; }
    std::string path = "../../www" + pagePath + ".html";

    std::ifstream page(path);
    if(page.is_open()){
        std::string line = "";

        while(std::getline(page, line)){
            pageStr += line + "\n";
        }
        page.close();
        return pageStr;
    }

    return "404";
}
