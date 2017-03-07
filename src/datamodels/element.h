#ifndef ELEMENT_H
#define ELEMENT_H

#include <QList>
#include "pin.h"

/**
 * @brief The Element class
 * Represents a scheme's element.
 */
class Element : public Serializable
{
protected:
    QString id;
    QString name, model;
    int height, width;
    QList <Pin> pins;

public:
    Element();
};

#endif // ELEMENT_H
