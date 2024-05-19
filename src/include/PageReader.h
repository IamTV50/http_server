#ifndef SRC_PAGEREADER_H
#define SRC_PAGEREADER_H

#include <string>

class PageReader{
public:
    std::string read404page();
    PageReader() = default;
    std::string readPage(std::string pagePath);
};

#endif //SRC_PAGEREADER_H
