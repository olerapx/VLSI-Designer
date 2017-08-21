#ifndef ROUTINGEXCEPTION_H
#define ROUTINGEXCEPTION_H

#include "exception/exception.h"

/**
 * @brief The RoutingException class
 * The instance of this class is thrown if an error during the routing process is occured.
 */
class RoutingException : public Exception
{
public:
    RoutingException(QString what): Exception(what) {}
};

#endif // ROUTINGEXCEPTION_H
