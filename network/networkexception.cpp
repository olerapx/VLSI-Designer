#include "networkexception.h"

NetworkException::NetworkException(QString what)
{
    this->error = what;
}
