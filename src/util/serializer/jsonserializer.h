#pragma once

#include <typeinfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "serializer.h"

/**
 * @brief The JsonSerializer class
 * Implements a JSON serialization and deserialization of objects.
 *
 * The object you need to be serialized or deserialized must be inherited from Serializable.
 * If the object's type is not supported, an IllegalArgumentException will be thrown.
 */
class JsonSerializer : public Serializer
{
public:
    JsonSerializer() {}
    virtual ~JsonSerializer() {}

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
    QJsonObject serializeUsedLibrary(QString libraryId, double version);
    QJsonObject serializeSchemeElement(SchemeElement el);
    QJsonObject serializeWire(Wire w);    

    QByteArray serializeGrid(Grid* g);
    QJsonObject serializeCell(Cell c);
    QJsonObject serializeWireData(WireData data);

    QByteArray serializeArchitecture(Architecture* a);


    Library* deserializeLibrary(QJsonObject obj);
    LibraryElement deserializeLibraryElement(QJsonObject obj);
    Pin deserializePin(QJsonObject obj);

    Scheme* deserializeScheme(QJsonObject obj);
    QPair<QString, double> deserializeUsedLibrary(QJsonObject obj);
    SchemeElement deserializeSchemeElement(QJsonObject obj);
    Wire deserializeWire(QJsonObject obj);    

    Grid* deserializeGrid(QJsonObject obj);
    Cell deserializeCell(QJsonObject obj);
    WireData deserializeWireData(QJsonObject obj);

    Architecture* deserializeArchitecture(QJsonObject obj);
};
