#ifndef SRC_PAGEREADER_H
#define SRC_PAGEREADER_H

#include <string>

class PageReader{
private:
    std::string read404page();

public:
    PageReader() = default;
    std::string readPage(std::string pagePath);
};

#endif //SRC_PAGEREADER_H
