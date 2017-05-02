#include <QtTest>
#include <QCoreApplication>
#include <QFile>

#include <util/jsonserializer.h>

class UnsupportedSerializable: public Serializable
{
public:
    virtual ~UnsupportedSerializable(){}
};

class JsonSerializerTest : public QObject
{
    Q_OBJECT

    QString datamodelsDir;

public:
    JsonSerializerTest();
    ~JsonSerializerTest();

private slots:
    void serializeTest();

    void serializeLibraryTest();
    void serializeSchemeTest();
    void serializeGridTest();
    void serializeArchitectureTest();


    void deserializeTest();

    void deserializeLibraryTest();
    void deserializeSchemeTest();
    void deserializeGridTest();
    void deserializeArchitectureTest();
};

JsonSerializerTest::JsonSerializerTest()
{
    QDir dir (QApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();

    datamodelsDir = dir.path();
}

JsonSerializerTest::~JsonSerializerTest()
{

}

void JsonSerializerTest::serializeTest()
{
    JsonSerializer json;

    Library* l = new Library("test_id", 10.0);
    json.serialize(l);
    delete l;

    UnsupportedSerializable* d = new UnsupportedSerializable();
    QVERIFY_EXCEPTION_THROWN(json.serialize(d), IllegalArgumentException);
    delete d;
}

void JsonSerializerTest::serializeLibraryTest()
{
    Library* l = new Library("test_id", 10.001);
    l->setName("Test standard cell library с кириллическими знаками");

    LibraryElement el("el", 10, 100);
    el.setModel("el model");
    el.setName("el name");

    Pin pin("pin id", 1, 0, PinType::Input);
    el.getPins().append(pin);
    l->getElements().append(el);

    LibraryElement el2("el2", 1, 1);
    l->getElements().append(el2);

    JsonSerializer json;
    QByteArray arr = json.serialize(l);
    QJsonDocument j = QJsonDocument::fromJson(arr);
    QJsonObject obj = j.object().value("library").toObject();

    QVERIFY(obj.value("id").toString() == l->getId());
    QVERIFY (obj.value("version").toDouble() == l->getVersion());
    QVERIFY(obj.value("name").toString() == l->getName());
    QVERIFY(obj.value("elements").toArray().size() == 2);

    QJsonObject elJson = obj.value("elements").toArray().at(0).toObject();
    QVERIFY(elJson.value("id").toString() == el.getId());
    QVERIFY(elJson.value("height").toInt() == el.getHeight());
    QVERIFY(elJson.value("width").toInt() == el.getWidth());
    QVERIFY(elJson.value("model").toString() == el.getModel());
    QVERIFY(elJson.value("name").toString() == el.getName());
    QVERIFY(elJson.value("pins").toArray().size() == 1);

    QJsonObject pinJson = elJson.value("pins").toArray().at(0).toObject();
    QVERIFY(pinJson.value("id").toString() == pin.getId());
    QVERIFY(pinJson.value("x").toInt() == pin.getX());
    QVERIFY(pinJson.value("y").toInt() == pin.getY());
    QVERIFY(pinJson.value("type").toString() == "input");

    elJson = obj.value("elements").toArray().at(1).toObject();
    QVERIFY(elJson.value("pins").toArray().size() == 0);

    delete l;
}

void JsonSerializerTest::serializeSchemeTest()
{
    Scheme* s = new Scheme();

    SchemeElement el1("library id", "aoi43242", Q_INT64_C(9223372036854775807));
    SchemeElement el2("23123", "NOR", 2);
    s->getElements().append(el1);
    s->getElements().append(el2);

    Wire w(Q_INT64_C(9223372036854775807), "a", Q_INT64_C(2), "z", WireType::Outer, Q_INT64_C(0));
    s->getWires().append(w);

    JsonSerializer json;
    QByteArray arr = json.serialize(s);
    QJsonDocument j = QJsonDocument::fromJson(arr);
    QJsonObject obj = j.object().value("scheme").toObject();

    QVERIFY(obj.value("elements").toArray().size() == 2);
    QVERIFY(obj.value("wires").toArray().size() == 1);

    QJsonObject element = obj.value("elements").toArray().at(0).toObject();
    QVERIFY(element.value("library-id").toString() == el1.getLibraryId());
    QVERIFY(element.value("element-id").toString() == el1.getElementId());

    qint64 index = element.value("index").toString().toLongLong();
    QVERIFY(index == el1.getIndex());

    QJsonObject wire = obj.value("wires").toArray().at(0).toObject();

    index = wire.value("src-index").toString().toLongLong();
    QVERIFY(index == w.getSrcIndex());
    QVERIFY(wire.value("src-pin-id").toString() == w.getSrcPinId());

    index = wire.value("dest-index").toString().toLongLong();
    QVERIFY(index == w.getDestIndex());
    QVERIFY(wire.value("dest-pin-id").toString() == w.getDestPinId());
    QVERIFY(wire.value("type").toString() == "outer");

    index = wire.value("index").toString().toLongLong();
    QVERIFY(index == w.getIndex());

    delete s;
}

void JsonSerializerTest::serializeGridTest()
{
    Grid* g = new Grid(10);

    g->getRoutedWires().append(1);
    g->getRoutedWires().append(0);
    g->getRoutedWires().append(Q_INT64_C(9223372036854775807));

    QList<Cell> firstRow, secondRow;

    Cell c1(CellType::Empty);
    firstRow.append(c1);
    Cell c2(CellType::LRU);
    firstRow.append(c2);

    Cell c3(CellType::Element, Q_INT64_C(34535345345343));
    secondRow.append(c3);
    Cell c4(CellType::Pin, Q_INT64_C(1), "z");
    secondRow.append(c4);

    g->getCells().append(firstRow);
    g->getCells().append(secondRow);

    JsonSerializer json;
    QByteArray arr = json.serialize(g);
    QJsonDocument j = QJsonDocument::fromJson(arr);
    QJsonObject obj = j.object().value("grid").toObject();

    QVERIFY(obj.value("cells").toArray().size() == 2);
    QVERIFY(obj.value("routed-wires").toArray().size() == 3);

    RoutedWireIndex index = obj.value("routed-wires").toArray().at(2).toString().toLongLong();
    QVERIFY(index == Q_INT64_C(9223372036854775807));

    QVERIFY(obj.value("initial-level").toInt() == 10);

    QJsonArray row1 = obj.value("cells").toArray().at(0).toArray();
    QJsonArray row2 = obj.value("cells").toArray().at(1).toArray();

    QJsonObject cell = row1.at(0).toObject();
    QVERIFY(cell.value("type").toString() == "empty");
    QVERIFY(!cell.contains("index"));
    QVERIFY(!cell.contains("pin-id"));

    cell = row1.at(1).toObject();
    QVERIFY(cell.value("type").toString() == "LRU");

    cell = row2.at(0).toObject();
    QVERIFY(cell.value("type").toString() == "element");

    index = cell.value("index").toString().toLongLong();
    QVERIFY(index == c3.getIndex());
    QVERIFY(!cell.contains("pin-id"));

    cell = row2.at(1).toObject();
    QVERIFY(cell.value("type").toString() == "pin");

    index = cell.value("index").toString().toLongLong();
    QVERIFY(index == c4.getIndex());
    QVERIFY(cell.value("pin-id").toString() == c4.getPinId());

    delete g;
}

void JsonSerializerTest::serializeArchitectureTest()
{
    Architecture* a = new Architecture(DistributionType::Default);
    a->getModel().append(1);
    a->getModel().append(2);
    a->getModel().append(4);

    JsonSerializer json;
    QByteArray arr = json.serialize(a);
    QJsonDocument j = QJsonDocument::fromJson(arr);
    QJsonObject obj = j.object().value("architecture").toObject();

    QVERIFY(obj.value("model").toArray().size() == 3);
    QVERIFY(obj.value("model").toArray().at(1) == 2);
    QVERIFY(obj.value("distribution-type").toString() == "default");

    delete a;
}


void JsonSerializerTest::deserializeTest()
{
    JsonSerializer json;

    QByteArray notJson = "dummy text";
    QVERIFY_EXCEPTION_THROWN(json.deserialize(notJson), IllegalArgumentException);

    QByteArray invalidTypeJson = "{ \"dummy\": { }}";
    QVERIFY_EXCEPTION_THROWN(json.deserialize(invalidTypeJson), IllegalArgumentException);

    QByteArray normalJson = "{ \"architecture\": { \"distribution-type\": \"default\" }}";
    QVERIFY(json.deserialize(normalJson));
}

void JsonSerializerTest::deserializeLibraryTest()
{
    JsonSerializer json;
    Library* l;
    {
        QFile f(datamodelsDir+"/datamodels/library.json");
        f.open(QIODevice::ReadOnly);

        l = static_cast<Library*>(json.deserialize(f.readAll()));
        f.close();
    }

    QVERIFY(l->getName()=="Generic Standard Element Library");
    QVERIFY(l->getVersion() == 0.1);
    QVERIFY(l->getId() == "generic");
    QVERIFY(l->getElements().size() == 9);

    LibraryElement el = l->getElements().at(0);
    QVERIFY(el.getId()=="an2");
    QVERIFY(el.getModel()=="Generic");
    QVERIFY(el.getHeight() == 4);
    QVERIFY(el.getWidth() == 2);
    QVERIFY(el.getPins().size() == 3);

    Pin p = el.getPins().at(0);
    QVERIFY(p.getId()=="a");
    QVERIFY(p.getType() == PinType::Input);
    QVERIFY(p.getX()==0);
    QVERIFY(p.getY()==0);

    delete l;

    {
        QFile f (datamodelsDir+"/datamodels/library_invalid_pin_type.json");
        f.open(QIODevice::ReadOnly);
        QVERIFY_EXCEPTION_THROWN(json.deserialize(f.readAll()), IllegalArgumentException);
        f.close();
    }
}

void JsonSerializerTest::deserializeSchemeTest()
{
    JsonSerializer json;
    Scheme* s;
    {
        QFile f(datamodelsDir+"/datamodels/scheme.json");
        f.open(QIODevice::ReadOnly);

        s = static_cast<Scheme*>(json.deserialize(f.readAll()));
        f.close();
    }

    QVERIFY(s->getElements().size() == 3);
    QVERIFY(s->getWires().size() == 3);

    SchemeElement el = s->getElements().at(0);
    QVERIFY(el.getLibraryId() == "generic");
    QVERIFY(el.getElementId() == "an2");
    QVERIFY(el.getIndex() == 9223372036854775807);

    Wire w = s->getWires().at(0);
    QVERIFY(w.getSrcIndex() == 9223372036854775807);
    QVERIFY(w.getSrcPinId() == "z");
    QVERIFY(w.getDestIndex() == 1);
    QVERIFY(w.getDestPinId() == "a");
    QVERIFY(w.getType() == WireType::Inner);
    QVERIFY(w.getIndex() == 0);

    delete s;

    {
        QFile f(datamodelsDir+"/datamodels/scheme_invalid_wire_type.json");
        f.open(QIODevice::ReadOnly);
        QVERIFY_EXCEPTION_THROWN(json.deserialize(f.readAll()), IllegalArgumentException);
        f.close();
    }
}

void JsonSerializerTest::deserializeGridTest()
{
    JsonSerializer json;
    Grid* g;
    {
        QFile f(datamodelsDir+"/datamodels/grid.json");
        f.open(QIODevice::ReadOnly);

        g = static_cast<Grid*>(json.deserialize(f.readAll()));
        f.close();
    }

    QVERIFY(g->getCells().size() == 4);

    QList<Cell> list = g->getCells().at(0);
    QVERIFY(list.size() == 4);

    QList<RoutedWireIndex> routedWires = {0, 1};
    QVERIFY(g->getRoutedWires() == routedWires);
    QVERIFY(g->getInitialLevel() == 1);

    Cell emptyCell = g->getCells().at(0).at(0);
    QVERIFY(emptyCell.getType() == CellType::Empty);

    Cell elementCell = g->getCells().at(0).at(1);
    QVERIFY(elementCell.getType() == CellType::Element);
    QVERIFY(elementCell.getIndex() == 0);

    Cell pinCell = g->getCells().at(0).at(2);
    QVERIFY(pinCell.getType() == CellType::Pin);
    QVERIFY(pinCell.getIndex() == 0);
    QVERIFY(pinCell.getPinId() == "z");

    {
        QFile f(datamodelsDir+"/datamodels/grid_invalid_cell_type.json");
        f.open(QIODevice::ReadOnly);
        QVERIFY_EXCEPTION_THROWN(json.deserialize(f.readAll()), IllegalArgumentException);
        f.close();
    }

    delete g;
}

void JsonSerializerTest::deserializeArchitectureTest()
{
    JsonSerializer json;
    Architecture* a;
    {
        QFile f(datamodelsDir+"/datamodels/architecture.json");
        f.open(QIODevice::ReadOnly);

        a = static_cast<Architecture*>(json.deserialize(f.readAll()));
        f.close();
    }

    QList<ClientsNumber> model = {2, 3, 2};

    QVERIFY(a->getModel() == model);
    QVERIFY(a->getDistributionType() == DistributionType::Greedy);

    {
       QFile f(datamodelsDir+"/datamodels/architecture_invalid_distribution_type.json");
       f.open(QIODevice::ReadOnly);
       QVERIFY_EXCEPTION_THROWN(json.deserialize(f.readAll()), IllegalArgumentException);
       f.close();
    }

    delete a;
}

QTEST_MAIN(JsonSerializerTest)

#include "tst_jsonserializertest.moc"
