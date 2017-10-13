#include <QtTest>
#include <QCoreApplication>
#include <QFile>

#include <util/serializer/jsonserializer.h>

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
    ~JsonSerializerTest() {}

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
    dir.cdUp();

    datamodelsDir = dir.path() + "/datamodels";
}

void JsonSerializerTest::serializeTest()
{
    JsonSerializer json;

    Library* l = new Library("test_id", Version(10, 0, 0, 0));
    json.serialize(l);
    delete l;

    UnsupportedSerializable* d = new UnsupportedSerializable();
    QVERIFY_EXCEPTION_THROWN(json.serialize(d), IllegalArgumentException);
    delete d;
}

void JsonSerializerTest::serializeLibraryTest()
{
    Library* l = new Library("test_id", Version(10, 0, 0, 1));
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
    QVERIFY (Version(obj.value("version").toString()) == l->getVersion());
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

    s->getUsedLibraries().append(QPair<QString, Version>("lib1", Version(2, 0, 0, 0)));
    s->getUsedLibraries().append(QPair<QString, Version>("lib2", Version(1, 5, 0, 0)));
    s->getUsedLibraries().append(QPair<QString, Version>("lib3", Version(0, 13, 0, 0)));

    SchemeElement el1("library id", "aoi43242", Q_INT64_C(9223372036854775807));
    el1.setAlias("GFA");

    SchemeElement el2("23123", "NOR", 2);
    s->getElements().append(el1);
    s->getElements().append(el2);

    Wire w(Q_INT64_C(9223372036854775807), "a", Q_INT64_C(2), "z", WireType::Outer, Q_INT64_C(0));
    s->getWires().append(w);

    JsonSerializer json;
    QByteArray arr = json.serialize(s);
    QJsonDocument j = QJsonDocument::fromJson(arr);
    QJsonObject obj = j.object().value("scheme").toObject();

    QVERIFY(obj.value("used-libraries").toArray().size() == 3);
    QVERIFY(obj.value("elements").toArray().size() == 2);
    QVERIFY(obj.value("wires").toArray().size() == 1);

    QJsonObject usedLibrary = obj.value("used-libraries").toArray().at(0).toObject();
    QVERIFY(usedLibrary.value("library-id").toString() == s->getUsedLibraries()[0].first);
    QVERIFY(Version(usedLibrary.value("version").toString()) == s->getUsedLibraries()[0].second);

    usedLibrary = obj.value("used-libraries").toArray().at(2).toObject();
    QVERIFY(usedLibrary.value("library-id").toString() == s->getUsedLibraries()[2].first);
    QVERIFY(Version(usedLibrary.value("version").toString()) == s->getUsedLibraries()[2].second);

    QJsonObject element = obj.value("elements").toArray().at(0).toObject();
    QVERIFY(element.value("library-id").toString() == el1.getLibraryId());
    QVERIFY(element.value("element-id").toString() == el1.getElementId());
    QVERIFY(element.value("alias").toString() == el1.getAlias());

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
    Grid* g = new Grid();

    g->getRoutedWires().append(1);
    g->getRoutedWires().append(0);
    g->getRoutedWires().append(Q_INT64_C(9223372036854775807));

    g->getWiresData().append(WireData(Q_INT64_C(34535345345343), QPoint(1, 2), QPoint(3, 4), WirePosition::Internal));
    g->getWiresData().append(WireData(Q_INT64_C(3), QPoint(1, 1), QPoint(5, 5), WirePosition::External));

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

    QVERIFY(obj.value("wires-data").toArray().size() == 2);

    QJsonObject wireData1 = obj.value("wires-data").toArray().at(0).toObject();
    QVERIFY(wireData1.value("index").toString().toLongLong() == Q_INT64_C(34535345345343));
    QVERIFY(wireData1.value("src-x").toInt() == 1);
    QVERIFY(wireData1.value("src-y").toInt() == 2);
    QVERIFY(wireData1.value("dest-x").toInt() == 3);
    QVERIFY(wireData1.value("dest-y").toInt() == 4);
    QVERIFY(wireData1.value("position").toString() == "internal");

    QJsonObject wireData2 = obj.value("wires-data").toArray().at(1).toObject();
    QVERIFY(wireData2.value("position").toString() == "external");

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
    AlgorithmIndexes i;
    i.setCompositionAlgorithmIndex(0);
    i.setPrimaryPlacementAlgorithmIndex(2);
    i.setSecondaryPlacementAlgorithmIndex(3);
    i.setRoutingAlgorithmIndex(4);

    AlgorithmParameters p;
    p.setExpandingCoefficient(1.5);
    p.setMaxExtensionAttempts(100);
    p.setMaxExtensionAttemptsForWire(10);

    Architecture* a = new Architecture(DistributionType::Default, i, p);
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

    QJsonObject obj1 = obj.value("algorithm-indexes").toObject();

    QVERIFY(obj1.value("composition").toInt() == 0);
    QVERIFY(obj1.value("decomposition").toInt() == 0);
    QVERIFY(obj1.value("primary-placement").toInt() == 2);
    QVERIFY(obj1.value("secondary-placement").toInt() == 3);
    QVERIFY(obj1.value("routing").toInt() == 4);

    obj1 = obj.value("algorithm-parameters").toObject();
    QVERIFY(obj1.value("expanding-coefficient").toDouble() == 1.5);
    QVERIFY(obj1.value("max-extension-attempts").toInt() == 100);
    QVERIFY(obj1.value("max-extension-attempts-for-wire").toInt() == 10);

    delete a;
}


void JsonSerializerTest::deserializeTest()
{
    JsonSerializer json;

    QByteArray notJson = "dummy text";
    QVERIFY_EXCEPTION_THROWN(json.deserialize(notJson), IllegalArgumentException);

    QByteArray invalidTypeJson = "{ \"dummy\": { }}";
    QVERIFY_EXCEPTION_THROWN(json.deserialize(invalidTypeJson), IllegalArgumentException);

    QByteArray normalJson = "{ \"architecture\": { \"distribution-type\": \"default\", "
                            "\"algorithm-parameters\": {\"expanding-coefficient\": 2.5,\"max-extension-attempts\": 10} }}";
    QVERIFY(json.deserialize(normalJson));
}

void JsonSerializerTest::deserializeLibraryTest()
{
    JsonSerializer json;
    Library* l;
    {
        QFile f(datamodelsDir+"/library.json");
        f.open(QIODevice::ReadOnly);

        l = dynamic_cast<Library*>(json.deserialize(f.readAll()));
        f.close();
    }

    QVERIFY(l->getName()=="Generic Standard Element Library");
    QVERIFY(l->getVersion() == Version(0, 1, 1, 3));
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
        QFile f (datamodelsDir+"/library_invalid_pin_type.json");
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
        QFile f(datamodelsDir+"/scheme.json");
        f.open(QIODevice::ReadOnly);

        s = dynamic_cast<Scheme*>(json.deserialize(f.readAll()));
        f.close();
    }

    QVERIFY(s->getUsedLibraries().size() == 2);
    QVERIFY(s->getElements().size() == 3);
    QVERIFY(s->getWires().size() == 3);

    QPair<QString, Version> ul = s->getUsedLibraries().at(1);
    QVERIFY(ul.first == "another");
    QVERIFY(ul.second == Version(0, 3, 2, 45));

    SchemeElement el = s->getElements().at(0);
    QVERIFY(el.getLibraryId() == "generic");
    QVERIFY(el.getElementId() == "an2");
    QVERIFY(el.getIndex() == 9223372036854775807);
    QVERIFY(el.getAlias() == "");

    Wire w = s->getWires().at(0);
    QVERIFY(w.getSrcIndex() == 9223372036854775807);
    QVERIFY(w.getSrcPinId() == "z");
    QVERIFY(w.getDestIndex() == 1);
    QVERIFY(w.getDestPinId() == "a");
    QVERIFY(w.getType() == WireType::Inner);
    QVERIFY(w.getIndex() == 0);

    delete s;

    {
        QFile f(datamodelsDir+"/scheme_invalid_wire_type.json");
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
        QFile f(datamodelsDir+"/grid.json");
        f.open(QIODevice::ReadOnly);

        g = dynamic_cast<Grid*>(json.deserialize(f.readAll()));
        f.close();
    }

    QVERIFY(g->getHeight() == 4);

    QList<Cell> list = g->getCells().at(0);
    QVERIFY(list.size() == 4);

    QList<RoutedWireIndex> routedWires = {0, 1};
    QVERIFY(g->getRoutedWires() == routedWires);

    QList<WireData> wiresData;
    wiresData.append(WireData(Q_INT64_C(9223372036854775807), QPoint(3, 5), QPoint(10, 15), WirePosition::Internal));
    wiresData.append(WireData(Q_INT64_C(1), QPoint(1, 2), QPoint(0, 0), WirePosition::External));

    QVERIFY(g->getWiresData() == wiresData);

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
        QFile f(datamodelsDir+"/grid_invalid_cell_type.json");
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
        QFile f(datamodelsDir+"/architecture.json");
        f.open(QIODevice::ReadOnly);

        a = dynamic_cast<Architecture*>(json.deserialize(f.readAll()));
        f.close();
    }

    QList<ClientsNumber> model = {2, 3, 2};

    QVERIFY(a->getModel() == model);
    QVERIFY(a->getDistributionType() == DistributionType::Greedy);

    {
       QFile f(datamodelsDir+"/architecture_invalid_distribution_type.json");
       f.open(QIODevice::ReadOnly);
       QVERIFY_EXCEPTION_THROWN(json.deserialize(f.readAll()), IllegalArgumentException);
       f.close();
    }

    QVERIFY(a->getAlgorithmIndexes().getCompositionAlgorithmIndex() == 0);
    QVERIFY(a->getAlgorithmIndexes().getDecompositionAlgorithmIndex() == 1);
    QVERIFY(a->getAlgorithmIndexes().getPrimaryPlacementAlgorithmIndex() == 2);
    QVERIFY(a->getAlgorithmIndexes().getSecondaryPlacementAlgorithmIndex() == 3);
    QVERIFY(a->getAlgorithmIndexes().getRoutingAlgorithmIndex() == 4);

    QVERIFY(a->getAlgorithmParameters().getExpandingCoefficient() == 2.5);
    QVERIFY(a->getAlgorithmParameters().getMaxExtensionAttempts() == 100);
    QVERIFY(a->getAlgorithmParameters().getMaxExtensionAttemptsForWire() == 10);

    delete a;
}

QTEST_MAIN(JsonSerializerTest)

#include "tst_jsonserializertest.moc"
