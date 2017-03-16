#ifndef CLIENTSNUMBER_H
#define CLIENTSNUMBER_H

#include "exception/illegalargumentexception.h"

/**
 * @brief The ClientsNumber class
 */
class ClientsNumber
{
private:
    int value;

    void setValue (int number);
public:
    ClientsNumber(int number);

    operator int() const {return this->value;}
    int getValue() const {return this->value;}
};

#endif // CLIENTSNUMBER_H
