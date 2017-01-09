#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

#include <QException>
#include <QString>

class NetworkException: public QException
{
public:
    NetworkException(QString what);
    QString what() { return this->error; }

    void raise() const { throw *this; }
    NetworkException *clone() const { return new NetworkException(*this); }
private:
    QString error;
};

#endif // NETWORKEXCEPTION_H
