#ifndef ELEMENT_H
#define ELEMENT_H

#include <QList>

#include "pin.h"

/**
 * @brief The Element class
 * Represents a library element.
 */
class LibraryElement : public Serializable
{
protected:
    QString id; /// Should be unique across the given library.
    QString name, model;
    int height, width;
    QList<Pin> pins;

public:
    LibraryElement(QString id, int height, int width);
    virtual ~LibraryElement() {}

    void setId(QString id);

    void setName(QString name);
    void setModel(QString model);

    void setHeight(int height);
    void setWidth(int width);


    QString getId() const { return id; }

    QString getName() const { return name; }
    QString getModel() const { return model; }

    int getHeight() const { return height; }
    int getWidth() const { return width; }

    QList<Pin>& getPins() { return pins; }

    bool operator ==(const LibraryElement& other);
};

#endif // ELEMENT_H
