#ifndef IOEXCEPTION_H
#define IOEXCEPTION_H

#include "exception.h"

/**
 * @brief The IllegalArgumentException class
 * The instance of this class is thrown if an error of reading or writing to some device or file is occured.
 */
class IOException: public Exception
{
public:
    IOException(QString what): Exception(what){}
};

#endif // IOEXCEPTION_H
