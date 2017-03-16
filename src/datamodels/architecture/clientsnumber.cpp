#include "clientsnumber.h"

ClientsNumber::ClientsNumber(int number)
{
    setValue(number);
}

void ClientsNumber::setValue(int number)
{
    if (number <= 0)
        throw IllegalArgumentException("Clients number must be positive");

    this->value = number;
}
