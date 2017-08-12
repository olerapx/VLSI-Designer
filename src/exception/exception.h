#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>
#include <QString>

/**
 * @brief The Exception class
 * Base class for all exceptions.
 */
class Exception: public QException
{
protected:
    QString error;

public:
    Exception(QString what);
    QString what() { return this->error; }

    void raise() const { throw *this; }
    Exception* clone() const { return new Exception(*this); }
};

#endif // EXCEPTION_H
