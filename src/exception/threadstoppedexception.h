#ifndef THREADSTOPPEDEXCEPTION_H
#define THREADSTOPPEDEXCEPTION_H

#include "exception.h"

/**
 * @brief The ThreadStoppedException class
 * The instance of this class is thrown when the thread is stopped to quit its main executing function.
 * That exception should not be considered as error when the function is called from another thread.
 * When the function is called directly, this exception must be processed.
 */
class ThreadStoppedException: public Exception
{
public:
    ThreadStoppedException(QString what): Exception(what){}
};

#endif // THREADSTOPPEDEXCEPTION_H
