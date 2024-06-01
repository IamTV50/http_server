#include "../include/ImageReader.h"
#include <fstream>

std::string ImageReader::readImage(const std::string &filepath) {
    std::ifstream imgFile("../../www" + filepath, std::ios::binary | std::ios::ate);

    if(!imgFile.is_open()){
        return "404"; //error opening the file
    }

    std::streamsize imgSize = imgFile.tellg();
    imgFile.seekg(0, std::ios::beg);

    std::string buffer(imgSize, '\0');
    if(!imgFile.read(&buffer[0], imgSize)){
        return "500"; //couldn't read file
    }

    imgFile.close();
    return buffer;
}