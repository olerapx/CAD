#ifndef HGRAPHEXCEPTION_H
#define HGRAPHEXCEPTION_H

#include <string>

class HGraphException
{
private:
    std::string error;

public:
    HGraphException(std::string what);

    std::string getError() {return this->error;}
};

#endif // HGRAPHEXCEPTION_H
