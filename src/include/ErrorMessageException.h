#ifndef SRC_ERRORMESSAGEEXCEPTION_H
#define SRC_ERRORMESSAGEEXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

using std::exception;
using std::cout;
using std::string;

class ErrorMessageException : public exception{
private:
    string msg;

public:
    ErrorMessageException(string message = ""){ msg = message; };
    const char * what() const noexcept override{
        cout<<msg;
    }
};

#endif //SRC_ERRORMESSAGEEXCEPTION_H
