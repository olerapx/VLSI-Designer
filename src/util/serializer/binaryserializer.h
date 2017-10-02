#pragma once

#include <typeinfo>
#include <QDataStream>

#include "serializer.h"

/**
 * @brief The BinarySerializer class
 * Implements a binary serialization and deserialization of objects.
 *
 * The object you need to be serialized or deserialized must be inherited from Serializable.
 * If the object's type is not supported, an IllegalArgumentException will be thrown.
 */
class BinarySerializer : public Serializer
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
    QByteArray serialize(Serializable* s) override;

    /**
     * @brief deserialize
     * Performs a deserializing of the object inherited from Serializable.
     * @param binaryData - the byte array.
     * @throw IllegalArgumentException
     * @return the pointer on the deserialized object.
     */
    Serializable* deserialize(QByteArray binaryData) override;

protected:
    QByteArray serializeLibrary(Library* l);
    QDataStream& serializeLibraryElement(LibraryElement el, QDataStream& stream);
    QDataStream& serializePin(Pin p, QDataStream& stream);

    QByteArray serializeScheme(Scheme* s);
    QDataStream& serializeUsedLibrary(QString libraryId, Version version, QDataStream& stream);
    QDataStream& serializeSchemeElement(SchemeElement el, QDataStream& stream);
    QDataStream& serializeWire(Wire w, QDataStream& stream);

    QByteArray serializeGrid(Grid* g);
    QDataStream& serializeCell(Cell c, QDataStream& stream);
    QDataStream& serializeWireData(WireData d, QDataStream& stream);

    QByteArray serializeArchitecture(Architecture* a);
    QDataStream& serializeAlgorithmIndexes(AlgorithmIndexes i, QDataStream& stream);
    QDataStream& serializeAlgorithmParameters(AlgorithmParameters p, QDataStream& stream);


    Library* deserializeLibrary(QDataStream& stream);
    LibraryElement deserializeLibraryElement(QDataStream& stream);
    Pin deserializePin(QDataStream& stream);

    Scheme* deserializeScheme(QDataStream& stream);
    QPair<QString, Version> deserializeUsedLibrary(QDataStream& stream);
    SchemeElement deserializeSchemeElement(QDataStream& stream);
    Wire deserializeWire(QDataStream& stream);

    Grid* deserializeGrid(QDataStream& stream);
    Cell deserializeCell(QDataStream& stream);
    WireData deserializeWireData(QDataStream& stream);

    Architecture* deserializeArchitecture(QDataStream& stream);
    AlgorithmIndexes deserializeAlgorithmIndexes(QDataStream& stream);
    AlgorithmParameters deserializeAlgorithmParameters(QDataStream& stream);
};
