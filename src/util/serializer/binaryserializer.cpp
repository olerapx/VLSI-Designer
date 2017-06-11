#include "binaryserializer.h"

QByteArray BinarySerializer::serialize(Serializable *s)
{
    const std::type_info& info = typeid(*s);
    if (info == typeid(Library))
        return serializeLibrary(static_cast<Library*>(s));
    else if (info == typeid(Scheme))
        return serializeScheme(static_cast<Scheme*>(s));
    else if (info == typeid(Grid))
        return serializeGrid(static_cast<Grid*>(s));
    else if (info == typeid(Architecture))
        return serializeArchitecture(static_cast<Architecture*>(s));
    else
        throw IllegalArgumentException(QObject::tr("The passed object's type is not supported: %1.").arg(info.name()));
}

QByteArray BinarySerializer::serializeLibrary(Library* l)
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << QString("library");

    stream << l->getId();
    stream << l->getVersion();
    stream << l->getName();

    stream << (qint32)l->getElements().size();
    for(LibraryElement el: l->getElements())
        serializeLibraryElement(el, stream);

    return array;
}

QDataStream& BinarySerializer::serializeLibraryElement(LibraryElement el, QDataStream& stream)
{
    stream << el.getId();
    stream << el.getName() << el.getModel();
    stream << (qint32)el.getHeight() << (qint32)el.getWidth();

    stream << (qint32)el.getPins().size();
    for(Pin p: el.getPins())
        serializePin(p, stream);

    return stream;
}

QDataStream& BinarySerializer::serializePin(Pin p, QDataStream& stream)
{
    stream << p.getId();
    stream << (qint32)p.getX() << (qint32)p.getY();
    stream << (qint32)p.getType();

    return stream;
}

QByteArray BinarySerializer::serializeScheme(Scheme* s)
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << QString("scheme");

    stream << (qint32)s->getElements().size();
    for(SchemeElement el: s->getElements())
        serializeSchemeElement(el, stream);

    stream << (qint32)s->getWires().size();
    for(Wire w: s->getWires())
        serializeWire(w, stream);

    return array;
}

QDataStream& BinarySerializer::serializeSchemeElement(SchemeElement el, QDataStream& stream)
{
    stream << el.getLibraryId();
    stream << el.getElementId();
    stream << el.getIndex();
    stream << el.getAlias();

    return stream;
}

QDataStream& BinarySerializer::serializeWire(Wire w, QDataStream& stream)
{
    stream << w.getSrcIndex();
    stream << w.getSrcPinId();

    stream << w.getDestIndex();
    stream << w.getDestPinId();

    stream << (qint32)w.getType();
    stream << w.getIndex();

    return stream;
}

QByteArray BinarySerializer::serializeGrid(Grid* g)
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << QString("grid");

    stream << (qint32)g->getInitialLevel();

    stream << (qint32)g->getCells().size();
    for(QList<Cell> list: g->getCells())
    {
        stream << (qint32)list.size();
        for(Cell c: list)
            serializeCell(c, stream);
    }

    stream << (qint32)g->getRoutedWires().size();
    for(RoutedWireIndex index: g->getRoutedWires())
        stream << index;

    return array;
}

QDataStream& BinarySerializer::serializeCell(Cell c, QDataStream& stream)
{
    stream << (qint32)c.getType();
    stream << c.getIndex();
    stream << c.getPinId();

    return stream;
}

QByteArray BinarySerializer::serializeArchitecture(Architecture *a)
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << QString("architecture");

    stream << (qint32)a->getDistributionType();

    stream << (qint32)a->getModel().size();
    for(ClientsNumber number: a->getModel())
        stream << number;

    return array;
}

Serializable* BinarySerializer::deserialize(QByteArray binaryData)
{
    QDataStream stream(&binaryData, QIODevice::ReadOnly);

    QString key;
    stream >> key;

    if (key == "library")
        return deserializeLibrary(stream);
    else if (key == "scheme")
        return deserializeScheme(stream);
    else if (key == "grid")
        return deserializeGrid(stream);
    else if (key == "architecture")
        return deserializeArchitecture(stream);
    else
        throw IllegalArgumentException(QObject::tr("The contained object is not supported or cannot be deserialized."));
}

Library* BinarySerializer::deserializeLibrary(QDataStream &stream)
{
    QString id;
    double version;
    QString name;

    stream >> id >> version >> name;

    Library* l = new Library(id, version);

    l->setName(name);

    qint32 size;
    stream >> size;
    l->getElements().reserve(size);
    for(int i=0; i<size; i++)
        l->getElements().append(deserializeLibraryElement(stream));

    return l;
}

LibraryElement BinarySerializer::deserializeLibraryElement(QDataStream& stream)
{
    QString id, name, model;
    qint32 height, width;

    stream >> id >> name >> model;
    stream >> height >> width;

    LibraryElement el(id, (int)height, (int)width);
    el.setName(name);
    el.setModel(model);

    qint32 size;
    stream >> size;

    el.getPins().reserve(size);
    for(int i=0; i<size; i++)
        el.getPins().append(deserializePin(stream));

    return el;
}

Pin BinarySerializer::deserializePin(QDataStream& stream)
{
    QString id;
    qint32 x, y;
    qint32 type;

    stream >> id;
    stream >> x >> y;
    stream >> type;

    return Pin(id, (int)x, (int)y, (PinType)type);
}

Scheme* BinarySerializer::deserializeScheme(QDataStream& stream)
{
    Scheme* s = new Scheme();

    qint32 size;
    stream >> size;

    s->getElements().reserve(size);
    for(int i=0; i<size; i++)
        s->getElements().append(deserializeSchemeElement(stream));

    stream >> size;

    s->getWires().reserve(size);
    for(int i=0; i<size; i++)
        s->getWires().append(deserializeWire(stream));

    return s;
}

SchemeElement BinarySerializer::deserializeSchemeElement(QDataStream& stream)
{
    QString libraryId;
    QString elementId;
    qint64 index;
    QString alias;

    stream >> libraryId >> elementId >> index >> alias;

    SchemeElement element(libraryId, elementId, index);
    element.setAlias(alias);

    return element;
}

Wire BinarySerializer::deserializeWire(QDataStream& stream)
{
    qint64 srcIndex;
    QString srcPinId;

    qint64 destIndex;
    QString destPinId;

    qint32 type;
    qint64 index;

    stream >> srcIndex >> srcPinId >> destIndex >> destPinId >> type >> index;

    return Wire(srcIndex, srcPinId, destIndex, destPinId, (WireType)type, index);
}

Grid* BinarySerializer::deserializeGrid(QDataStream &stream)
{
    qint32 initialLevel;
    stream >> initialLevel;

    Grid* g = new Grid((int)initialLevel);

    qint32 size;
    stream >> size;
    g->getCells().reserve(size);
    for(int i=0; i<size; i++)
    {
        g->getCells().append(QList<Cell>());

        qint32 listSize;
        stream >> listSize;
        g->getCells()[i].reserve(listSize);

        for(int j=0; j<listSize; j++)
            g->getCells()[i].append(deserializeCell(stream));
    }

    stream >> size;
    g->getRoutedWires().reserve(size);

    for(int i=0; i<size; i++)
    {
        qint64 index;
        stream >> index;
        g->getRoutedWires().append(index);
    }

    return g;
}

Cell BinarySerializer::deserializeCell(QDataStream& stream)
{
    qint32 type;
    qint64 index;
    QString pinId;

    stream >> type >> index >> pinId;

    return Cell((CellType)type, index, pinId);
}

Architecture* BinarySerializer::deserializeArchitecture(QDataStream &stream)
{
    qint32 type;
    stream >> type;

    Architecture* a = new Architecture((DistributionType)type);

    qint32 size;
    stream >> size;
    a->getModel().reserve(size);

    for(int i=0; i<size; i++)
    {
        qint32 number;
        stream >> number;
        a->getModel().append(number);
    }

    return a;
}
