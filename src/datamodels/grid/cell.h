#ifndef CELL_H
#define CELL_H

#include "src/datamodels/serializable.h"
#include "src/exception/illegalargumentexception.h"

enum class CellType
{
    Empty,
    Pin,
    Element,
    UD,
    LR,
    UL, UR, DL, DR,
    UDL, UDR, LRU, LRD,
    UDLR
};

/**
 * @brief The Cell class
 * Represents a grid cell.
 */
class Cell: public Serializable
{
protected:
    CellType type;
    qint64 index;
    QString pinId;

public:
    Cell (CellType type, qint64 index = 0, QString pinId = "");

    void setType (CellType type, qint64 index = 0, QString pinId = "");
    void setIndex (qint64 index);
    void setPinId (QString pinId);

    CellType getType() {return this->type;}
    qint64 getIndex() {return this->index;}
    QString getPinId() {return this->pinId;}
};

#endif // CELL_H
