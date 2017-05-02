#ifndef CLIENTSNUMBER_H
#define CLIENTSNUMBER_H

#include "exception/illegalargumentexception.h"

/**
 * @brief The ClientsNumber class
 * Represents a number of clients on some level of used model described in architecture.
 */
class ClientsNumber
{
private:
    int value;

    void setValue(int number);

public:
    ClientsNumber(int number);

    operator int() const { return value; }
    int getValue() const { return value; }
};

#endif // CLIENTSNUMBER_H
