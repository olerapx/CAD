#include "hgraphexception.h"

HGraphException::HGraphException(std::string what)
{
    this->error = what;
}
