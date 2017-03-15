#ifndef ELEMENT_H
#define ELEMENT_H

#include <QList>
#include "pin.h"

/**
 * @brief The Element class
 * Represents a library element.
 * Note that an instance of this class will delete all contained pins in destructor.
 */
class Element : public Serializable
{
protected:
    QString id; /// Should be unique across the given library.
    QString name, model;
    int height, width;
    QList <Pin*> pins;

public:
    Element(QString id, int height, int width);
    virtual ~Element();

    void setId (QString id);
    void setName (QString name);
    void setModel (QString model);
    void setHeight (int height);
    void setWidth (int width);

    QString getId() {return id;}
    QString getName() {return name;}
    QString getModel() {return model;}
    int getHeight() {return height;}
    int getWidth() {return width;}
    QList <Pin*>& getPins() {return pins;}
};

#endif // ELEMENT_H
