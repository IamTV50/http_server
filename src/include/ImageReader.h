#ifndef SRC_IMAGEREADER_H
#define SRC_IMAGEREADER_H

#include <vector>
#include <string>

class ImageReader{
public:
    ImageReader() = default;
    std::string readImage(const std::string& filepath);
};

#endif //SRC_IMAGEREADER_H
