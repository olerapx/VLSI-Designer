#ifndef ILLEGALARGUMENTEXCEPTION_H
#define ILLEGALARGUMENTEXCEPTION_H

#include "exception.h"

/**
 * @brief The IllegalArgumentException class
 * The instance of this class is thrown if an illegal argument is passed to some function.
 */
class IllegalArgumentException: public Exception
{
public:
    IllegalArgumentException(QString what): Exception(what){}
};

#endif // ILLEGALARGUMENTEXCEPTION_H
