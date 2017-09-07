#pragma once

#include "exception/exception.h"

/**
 * @brief The NetworkException class
 * The instance of this class is thrown if a network error is occured.
 */
class NetworkException : public Exception
{
public:
    NetworkException(QString what): Exception(what) {}
};
