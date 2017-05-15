#ifndef BINARYSERIALIZER_H
#define BINARYSERIALIZER_H

#include <typeinfo>
#include <QDataStream>

#include "serializer.h"

class BinarySerializer: public Serializer
{
public:
    BinarySerializer();

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


    Library* deserializeLibrary(QByteArray array);
    LibraryElement deserializeLibraryElement(QDataStream& stream);
    Pin deserializePin(QDataStream& stream);

    Scheme* deserializeScheme(QByteArray array);
    SchemeElement deserializeSchemeElement(QDataStream& stream);
    Wire deserializeWire(QDataStream& stream);

    Grid* deserializeGrid(QByteArray array);
    Cell deserializeCell(QDataStream& stream);

    Architecture* deserializeArchitecture(QByteArray array);
};

#endif // BINARYSERIALIZER_H
