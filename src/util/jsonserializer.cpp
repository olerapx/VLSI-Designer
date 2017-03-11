#include "jsonserializer.h"

JsonSerializer::JsonSerializer()
{

}

Serializable JsonSerializer::deserialize(QByteArray jsonData)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if(!doc.isObject())
        throw IllegalArgumentException ("Invalid JSON");

    QJsonObject obj = doc.object();
    QString key = obj.keys()[0];

    if (key == "library")
        return deserializeLibrary(obj.value("library").toObject());
    else
        throw IllegalArgumentException ("The contained JSON object is not supported or cannot be deserialized directly");
}

Library JsonSerializer::deserializeLibrary (QJsonObject obj)
{
    QString id = obj.value("id").toString();
    double version = obj.value("version").toDouble(-1.0);

    Library library(id, version);
    library.setName(obj.value("name").toString());

    QJsonArray elements = obj.value("elements").toArray();
    for(QJsonValue val: elements)
        library.getElements().append(deserializeElement(val.toObject()));

    return library;
}

Element* JsonSerializer::deserializeElement (QJsonObject obj)
{
    QString id = obj.value("id").toString();

    int height = obj.value("height").toInt(-1);
    int width = obj.value("width").toInt(-1);

    Element* element = new Element(id, height, width);
    element->setName(obj.value("name").toString());
    element->setModel(obj.value("model").toString());

    QJsonArray pins = obj.value("pins").toArray();
    for (QJsonValue val: pins)
        element->getPins().append(deserializePin(val.toObject()));

    return element;
}

Pin* JsonSerializer::deserializePin (QJsonObject obj)
{
    QString id = obj.value("id").toString();

    int x = obj.value("x").toInt(-1);
    int y = obj.value("y").toInt(-1);

    QString t = obj.value("type").toString();
    PinType type;
    if (t == "input")
        type = PinType::Input;
    else if (t == "output")
        type = PinType::Output;
    else
        throw IllegalArgumentException("Invalid pin type specified");

    return new Pin (id, x, y, type);
}
