#ifndef BINARYSERIALIZER_H
#define BINARYSERIALIZER_H

#include <typeinfo>
#include <QDataStream>

#include "serializer.h"

/**
 * @brief The BinarySerializer class
 * Implements a binary serialization and deserialization of an objects.
 *
 * The object you need to be serialized or deserialized must be inherited from Serializable.
 * If the object's type is not supported, an IllegalArgumentException will be thrown.
 */
class BinarySerializer: public Serializer
{
public:
    BinarySerializer() {}
    virtual ~BinarySerializer() {}

    /**
     * @brief serialize
     * Performs a serializing of the object inherited from Serializable.
     * @param s - the object.
     * @throw IllegalArgumentException
     * @return the byte array.
     */
    QByteArray serialize(Serializable* s);

    /**
     * @brief deserialize
     * Performs a deserializing of the object inherited from Serializable.
     * @param binaryData - the byte array.
     * @throw IllegalArgumentException
     * @return the pointer on the deserialized object.
     */
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
