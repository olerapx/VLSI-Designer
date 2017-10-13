#include "jsonserializer.h"

QByteArray JsonSerializer::serialize(Serializable* s)
{
    const std::type_info& info = typeid(*s);
    if (info == typeid(Library))
        return serializeLibrary(dynamic_cast<Library*>(s));
    else if (info == typeid(Scheme))
        return serializeScheme(dynamic_cast<Scheme*>(s));
    else if (info == typeid(Grid))
        return serializeGrid(dynamic_cast<Grid*>(s));
    else if (info == typeid(Architecture))
        return serializeArchitecture(dynamic_cast<Architecture*>(s));
    else
        throw IllegalArgumentException(QObject::tr("The passed object's type is not supported: %1.").arg(info.name()));
}

QByteArray JsonSerializer::serializeLibrary(Library* l)
{
    QJsonObject json;

    json["id"] = l->getId();
    json["version"] = l->getVersion().toString();
    json["name"] = l->getName();

    QJsonArray elements;
    for(LibraryElement el: l->getElements())
        elements.append(serializeLibraryElement(el));
    json["elements"] = elements;

    QJsonObject res;
    res["library"] = json;

    QJsonDocument doc(res);
    return doc.toJson();
}

QJsonObject JsonSerializer::serializeLibraryElement(LibraryElement el)
{
    QJsonObject json;

    json["id"] = el.getId();
    json["name"] = el.getName();
    json["model"] = el.getModel();
    json["height"] = el.getHeight();
    json["width"] = el.getWidth();

    QJsonArray pins;
    for(Pin pin: el.getPins())
        pins.append(serializePin(pin));

    json["pins"] = pins;

    return json;
}

QJsonObject JsonSerializer::serializePin(Pin p)
{
    QJsonObject json;

    json["id"] = p.getId();
    json["x"] = p.getX();
    json["y"] = p.getY();
    QString type = pinTypeMap.key(p.getType());
    json["type"] = type;

    return json;
}

QByteArray JsonSerializer::serializeScheme(Scheme* s)
{
    QJsonObject json;

    QJsonArray usedLibraries, elements, wires;

    for(QPair<QString, Version>& usedLibrary: s->getUsedLibraries())
        usedLibraries.append(serializeUsedLibrary(usedLibrary.first, usedLibrary.second));

    for(SchemeElement el: s->getElements())
        elements.append(serializeSchemeElement(el));

    for(Wire w: s->getWires())
        wires.append(serializeWire(w));

    json["used-libraries"] = usedLibraries;
    json["elements"] = elements;
    json["wires"] = wires;

    QJsonObject res;
    res["scheme"] = json;

    QJsonDocument doc(res);
    return doc.toJson();
}

QJsonObject JsonSerializer::serializeUsedLibrary(QString libraryId, Version version)
{
    QJsonObject json;

    json["library-id"] = libraryId;
    json["version"] = version.toString();

    return json;
}

QJsonObject JsonSerializer::serializeSchemeElement(SchemeElement el)
{
    QJsonObject json;

    json["library-id"] = el.getLibraryId();
    json["element-id"] = el.getElementId();
    json["index"] = QString::number(el.getIndex());
    json["alias"] = el.getAlias();

    return json;
}

QJsonObject JsonSerializer::serializeWire(Wire w)
{
    QJsonObject json;

    json["src-index"] = QString::number(w.getSrcIndex());
    json["src-pin-id"] = w.getSrcPinId();
    json["dest-index"] = QString::number(w.getDestIndex());
    json["dest-pin-id"] = w.getDestPinId();

    QString type = wireTypeMap.key(w.getType());
    json["type"] = type;
    json["index"] = QString::number(w.getIndex());

    return json;
}

QByteArray JsonSerializer::serializeGrid(Grid* g)
{
    QJsonObject json;
    QJsonArray jsonCells;

    for (QList<Cell> row: g->getCells())
    {
        QJsonArray jsonRow;
        for(Cell c: row)
            jsonRow.append(serializeCell(c));
        jsonCells.append(jsonRow);
    }
    json["cells"] = jsonCells;

    QJsonArray routedWires;

    for(RoutedWireIndex i: g->getRoutedWires())
        routedWires.append(QString::number(i.getValue()));

    json["routed-wires"] = routedWires;

    QJsonArray wiresData;

    for(WireData data: g->getWiresData())
        wiresData.append(serializeWireData(data));

    json["wires-data"] = wiresData;

    QJsonObject res;
    res["grid"] = json;

    QJsonDocument doc(res);
    return doc.toJson();
}

QJsonObject JsonSerializer::serializeCell(Cell c)
{
    QJsonObject json;

    json["type"] = cellTypeMap.key(c.getType());

    if (c.getType() == CellType::Element || c.getType() == CellType::Pin)
        json["index"] = QString::number(c.getIndex());

    if (c.getType() == CellType::Pin)
        json["pin-id"] = c.getPinId();

    return json;
}

QJsonObject JsonSerializer::serializeWireData(WireData data)
{
    QJsonObject json;

    json["index"] = QString::number(data.getIndex());

    json["src-x"] = data.getSrcCoord().x();
    json["src-y"] = data.getSrcCoord().y();

    if(data.getWirePosition() == WirePosition::Internal)
    {
        json["dest-x"] = data.getDestCoord().x();
        json["dest-y"] = data.getDestCoord().y();
    }
    else
    {
        json["dest-x"] = 0;
        json["dest-y"] = 0;
    }

    json["position"] = wirePositionMap.key(data.getWirePosition());

    return json;
}

QByteArray JsonSerializer::serializeArchitecture(Architecture* a)
{
    QJsonObject json;

    QJsonArray model;
    for (int i: a->getModel())
        model.append(i);

    json["model"] = model;
    json["distribution-type"] = distributionTypeMap.key(a->getDistributionType());

    QJsonObject indexes = serializeAlgorithmIndexes(a->getAlgorithmIndexes());
    json["algorithm-indexes"] = indexes;

    QJsonObject parameters = serializeAlgorithmParameters(a->getAlgorithmParameters());
    json["algorithm-parameters"] = parameters;

    QJsonObject res;
    res["architecture"] = json;

    QJsonDocument doc(res);
    return doc.toJson();
}

QJsonObject JsonSerializer::serializeAlgorithmIndexes(AlgorithmIndexes i)
{
    QJsonObject json;

    json["composition"] = i.getCompositionAlgorithmIndex();
    json["decomposition"] = i.getDecompositionAlgorithmIndex();
    json["primary-placement"] = i.getPrimaryPlacementAlgorithmIndex();
    json["secondary-placement"] = i.getSecondaryPlacementAlgorithmIndex();
    json["routing"] = i.getRoutingAlgorithmIndex();

    return json;
}

QJsonObject JsonSerializer::serializeAlgorithmParameters(AlgorithmParameters p)
{
    QJsonObject json;

    json["expanding-coefficient"] = p.getExpandingCoefficient();
    json["max-extension-attempts"] = p.getMaxExtensionAttempts();
    json["max-extension-attempts-for-wire"] = p.getMaxExtensionAttemptsForWire();

    return json;
}

Serializable* JsonSerializer::deserialize(QByteArray jsonData)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if(!doc.isObject())
        throw IllegalArgumentException(QObject::tr("Invalid JSON."));

    QJsonObject obj = doc.object();
    QString key = obj.keys()[0];

    if (key == "library")
        return deserializeLibrary(obj.value("library").toObject());
    else if (key == "scheme")
        return deserializeScheme(obj.value("scheme").toObject());
    else if (key == "grid")
        return deserializeGrid(obj.value("grid").toObject());
    else if (key == "architecture")
        return deserializeArchitecture(obj.value("architecture").toObject());
    else
        throw IllegalArgumentException(QObject::tr("The contained JSON object is not supported or cannot be deserialized directly."));
}

Library* JsonSerializer::deserializeLibrary(QJsonObject obj)
{
    QString id = obj.value("id").toString();
    Version version(obj.value("version").toString());

    Library* library = new Library(id, version);
    library->setName(obj.value("name").toString());

    QJsonArray elements = obj.value("elements").toArray();
    for(QJsonValue val: elements)
        library->getElements().append(deserializeLibraryElement(val.toObject()));

    return library;
}

LibraryElement JsonSerializer::deserializeLibraryElement(QJsonObject obj)
{
    QString id = obj.value("id").toString();

    int height = obj.value("height").toInt(-1);
    int width = obj.value("width").toInt(-1);

    LibraryElement element(id, height, width);
    element.setName(obj.value("name").toString());
    element.setModel(obj.value("model").toString());

    QJsonArray pins = obj.value("pins").toArray();
    for (QJsonValue val: pins)
        element.getPins().append(deserializePin(val.toObject()));

    return element;
}

Pin JsonSerializer::deserializePin(QJsonObject obj)
{
    QString id = obj.value("id").toString();

    int x = obj.value("x").toInt(-1);
    int y = obj.value("y").toInt(-1);

    QString t = obj.value("type").toString();
    PinType type;
    if (pinTypeMap.contains(t))
        type = pinTypeMap[t];
    else
        throw IllegalArgumentException(QObject::tr("Invalid pin type specified, got: %1.").arg(t));

    return Pin(id, x, y, type);
}

Scheme* JsonSerializer::deserializeScheme(QJsonObject obj)
{
    Scheme* scheme = new Scheme();

    QJsonArray usedLibraries = obj.value("used-libraries").toArray();
    for(QJsonValue val: usedLibraries)
        scheme->getUsedLibraries().append(deserializeUsedLibrary(val.toObject()));

    QJsonArray elements = obj.value("elements").toArray();
    for (QJsonValue val: elements)
        scheme->getElements().append(deserializeSchemeElement(val.toObject()));

    QJsonArray wires = obj.value("wires").toArray();
    for (QJsonValue val: wires)
        scheme->getWires().append(deserializeWire(val.toObject()));

    return scheme;
}

QPair<QString, Version> JsonSerializer::deserializeUsedLibrary(QJsonObject obj)
{
    QString libraryId = obj.value("library-id").toString();
    Version version(obj.value("version").toString());

    return QPair<QString, Version>(libraryId, version);
}

SchemeElement JsonSerializer::deserializeSchemeElement(QJsonObject obj)
{
    QString libraryId = obj.value("library-id").toString();
    QString elementId = obj.value("element-id").toString();

    bool ok;
    qint64 index = obj.value("index").toString().toLongLong(&ok);
    if (!ok) index = -1;

    SchemeElement element(libraryId, elementId, index);

    if(obj.contains("alias"))
    {
        QString alias = obj.value("alias").toString();
        element.setAlias(alias);
    }

    return element;
}

Wire JsonSerializer::deserializeWire(QJsonObject obj)
{
    bool ok;
    qint64 srcIndex = obj.value("src-index").toString().toLongLong(&ok);
    if (!ok) srcIndex = -1;
    QString srcPinId = obj.value("src-pin-id").toString();

    qint64 destIndex = obj.value("dest-index").toString().toLongLong(&ok);
    if (!ok) destIndex = -1;
    QString destPinId = obj.value("dest-pin-id").toString();

    QString t = obj.value("type").toString();
    WireType type;
    if (wireTypeMap.contains(t))
        type = wireTypeMap[t];
    else
        throw IllegalArgumentException(QObject::tr("Invalid wire type specified, got: %1.").arg(t));

    qint64 index = obj.value("index").toString().toLongLong(&ok);
    if (!ok) index = -1;

    return Wire(srcIndex, srcPinId, destIndex, destPinId, type, index);
}

Grid* JsonSerializer::deserializeGrid(QJsonObject obj)
{
    Grid* grid = new Grid();

    QJsonArray cells = obj.value("cells").toArray();
    for (QJsonValue val: cells)
    {
        QList<Cell> rowList;
        QJsonArray row = val.toArray();
        for(QJsonValue cell: row)
        {
            rowList.append(deserializeCell(cell.toObject()));
        }
        grid->getCells().append(rowList);
    }

    QJsonArray routedWires = obj.value("routed-wires").toArray();
    for(QJsonValue val: routedWires)
    {
        bool ok;
        qint64 index = val.toString().toLongLong(&ok);
        if (!ok)
        {
            delete grid;
            throw IllegalArgumentException(QObject::tr("Invalid routed wire index, got: %1.").arg(QString::number(index)));
        }

        grid->getRoutedWires().append(index);
    }

    QJsonArray wiresData = obj.value("wires-data").toArray();
    for(QJsonValue val: wiresData)
        grid->getWiresData().append(deserializeWireData(val.toObject()));

    return grid;
}

Cell JsonSerializer::deserializeCell(QJsonObject obj)
{
    QString t = obj.value("type").toString();
    CellType type;
    if (cellTypeMap.contains(t))
        type = cellTypeMap[t];
    else
        throw IllegalArgumentException(QObject::tr("Invalid cell type specified, got: %1.").arg(t));

    bool ok;
    qint64 index = obj.value("index").toString().toLongLong(&ok);
    if (!ok) index = -1;

    QString pinId = obj.value("pin-id").toString();

    return Cell(type, index, pinId);
}

WireData JsonSerializer::deserializeWireData(QJsonObject obj)
{
    bool ok;
    qint64 index = obj.value("index").toString().toLongLong(&ok);
    if (!ok) index = -1;

    int srcX = obj.value("src-x").toInt(-1);
    int srcY = obj.value("src-y").toInt(-1);

    int destX = obj.value("dest-x").toInt(-1);
    int destY = obj.value("dest-y").toInt(-1);

    QString t = obj.value("position").toString();
    WirePosition position;

    if (wirePositionMap.contains(t))
        position = wirePositionMap[t];
    else
        throw IllegalArgumentException(QObject::tr("Invalid wire position specified, got: %1.").arg(t));

    return WireData(index, QPoint(srcX, srcY), QPoint(destX, destY), position);
}

Architecture* JsonSerializer::deserializeArchitecture(QJsonObject obj)
{
    QString t = obj.value("distribution-type").toString();
    DistributionType type;
    if (distributionTypeMap.contains(t))
        type = distributionTypeMap[t];
    else
        throw IllegalArgumentException(QObject::tr("Invalid distribution type specified, got: %1.").arg(t));

    QJsonObject json = obj.value("algorithm-indexes").toObject();
    AlgorithmIndexes i = deserializeAlgorithmIndexes(json);

    json = obj.value("algorithm-parameters").toObject();
    AlgorithmParameters p = deserializeAlgorithmParameters(json);

    Architecture* architecture = new Architecture(type, i, p);

    QJsonArray model = obj.value("model").toArray();
    for (QJsonValue val: model)
        architecture->getModel().append(val.toInt());

    return architecture;
}

AlgorithmIndexes JsonSerializer::deserializeAlgorithmIndexes(QJsonObject obj)
{
    AlgorithmIndexes res;

    res.setCompositionAlgorithmIndex(obj.value("composition").toInt());
    res.setDecompositionAlgorithmIndex(obj.value("decomposition").toInt());
    res.setPrimaryPlacementAlgorithmIndex(obj.value("primary-placement").toInt());
    res.setSecondaryPlacementAlgorithmIndex(obj.value("secondary-placement").toInt());
    res.setRoutingAlgorithmIndex(obj.value("routing").toInt());

    return res;
}

AlgorithmParameters JsonSerializer::deserializeAlgorithmParameters(QJsonObject obj)
{
    AlgorithmParameters res;

    res.setExpandingCoefficient(obj.value("expanding-coefficient").toDouble());
    res.setMaxExtensionAttempts(obj.value("max-extension-attempts").toInt());
    res.setMaxExtensionAttemptsForWire(obj.value("max-extension-attempts-for-wire").toInt());

    return res;
}
