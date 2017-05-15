#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <typeinfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "serializer.h"
#include "datamodels/architecture/architecture.h"
#include "datamodels/grid/grid.h"
#include "datamodels/library/library.h"
#include "datamodels/scheme/scheme.h"

/**
 * @brief The JsonSerializer class
 * Implements a JSON serialization and deserialization of of objects.
 *
 * The object you need to be serialized or deserialized must be inherited from Serializable.
 * If the object's type is not supported, an IllegalArgumentException will be thrown.
 */
class JsonSerializer: public Serializer
{
public:
    JsonSerializer();

    /**
     * @brief serialize
     * Performs a serializing of the object inherited from Serializable.
     * @param s - the object.
     * @throw IllegalArgumentException
     * @return the byte array represents the created JSON document.
     */
    QByteArray serialize(Serializable* s);

    /**
     * @brief deserialize
     * Performs a deserializing of the object inherited from Serializable.
     * @param jsonData - the byte array contains the JSON document.
     * @throw IllegalArgumentException
     * @return the pointer on the deserialized object.
     */
    Serializable* deserialize(QByteArray jsonData);

protected:
    QByteArray serializeLibrary(Library* l);
    QJsonObject serializeLibraryElement(LibraryElement el);
    QJsonObject serializePin(Pin p);

    QByteArray serializeScheme(Scheme* s);
    QJsonObject serializeSchemeElement(SchemeElement el);
    QJsonObject serializeWire(Wire w);

    QByteArray serializeGrid(Grid* g);
    QJsonObject serializeCell(Cell c);

    QByteArray serializeArchitecture(Architecture* a);


    Library* deserializeLibrary(QJsonObject obj);
    LibraryElement deserializeLibraryElement(QJsonObject obj);
    Pin deserializePin(QJsonObject obj);

    Scheme* deserializeScheme(QJsonObject obj);
    SchemeElement deserializeSchemeElement(QJsonObject obj);
    Wire deserializeWire(QJsonObject obj);

    Grid* deserializeGrid(QJsonObject obj);
    Cell deserializeCell(QJsonObject obj);

    Architecture* deserializeArchitecture(QJsonObject obj);
};

#endif // JSONSERIALIZER_H
