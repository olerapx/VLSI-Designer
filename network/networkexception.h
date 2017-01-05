#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

#include <QString>

class NetworkException
{
public:
    NetworkException(QString what);
    QString what() { return this->error; }

private:
    QString error;
};

#endif // NETWORKEXCEPTION_H
