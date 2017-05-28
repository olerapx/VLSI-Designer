#include "clientsnumber.h"

ClientsNumber::ClientsNumber(int number)
{
    setValue(number);
}

void ClientsNumber::setValue(int number)
{
    if (number <= 0)
        throw IllegalArgumentException(QObject::tr("Clients number must be positive, passed: %1.")
                                                   .arg(QString::number(number)));

    this->value = number;
}
