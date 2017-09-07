#pragma once

#include "exception.h"

/**
 * @brief The IOException class
 * The instance of this class is thrown if an error of reading or writing to some device or file is occured.
 */
class IOException: public Exception
{
public:
    IOException(QString what): Exception(what){}
};
