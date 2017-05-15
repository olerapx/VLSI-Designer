#ifndef BINARYSERIALIZER_H
#define BINARYSERIALIZER_H

#include <typeinfo>
#include <QDataStream>

#include "serializer.h"

class BinarySerializer: public Serializer
{
public:
    BinarySerializer();
    virtual ~BinarySerializer() {}

    QByteArray serialize(Serializable* s);

    Serializable* deserialize(QByteArray binaryData);

protected:
    QByteArray serializeLibrary(Library* l);
    QDataStream& serializeLibraryElement(LibraryElement el, QDataStream& stream);
    QDataStream& serializePin(Pin p, QDataStream& stream);

    QByteArray serializeScheme(Scheme* s);
    QDataStream& serializeSchemeElement(SchemeElement el, QDataStream& stream);
    QDataStream& serializeWire(Wire w, QDataStream& stream);

    QByteArray serializeGrid(Grid* g);
    QDataStream& serializeCell(Cell c, QDataStream& stream);

    QByteArray serializeArchitecture(Architecture* a);


    Library* deserializeLibrary(QDataStream& stream);
    LibraryElement deserializeLibraryElement(QDataStream& stream);
    Pin deserializePin(QDataStream& stream);

    Scheme* deserializeScheme(QDataStream& stream);
    SchemeElement deserializeSchemeElement(QDataStream& stream);
    Wire deserializeWire(QDataStream& stream);

    Grid* deserializeGrid(QDataStream& stream);
    Cell deserializeCell(QDataStream& stream);

    Architecture* deserializeArchitecture(QDataStream& stream);
};

#endif // BINARYSERIALIZER_H
