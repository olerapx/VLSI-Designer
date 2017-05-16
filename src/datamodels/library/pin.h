#ifndef PIN_H
#define PIN_H

#include "datamodels/serializable.h"

enum class PinType
{
    Input,
    Output
};

/**
 * @brief The Pin class
 * Represents a library element pin.
 */
class Pin : public Serializable
{
protected:
    QString id; /// Should be unique across the given element.
    int x, y; /// Coordinates of a pin. (0,0) is the top-left corner and x is horizontal axis.
    PinType type;

public:
    Pin(QString id, int x, int y, PinType type);
    virtual ~Pin() {}

    void setId(QString id);
    void setX(int x);
    void setY(int y);
    void setType(PinType type);

    QString getId() const { return id; }
    int getX() const { return x; }
    int getY() const { return y; }
    PinType getType() const { return type; }

    bool operator ==(const Pin& other);
};

#endif // PIN_H
