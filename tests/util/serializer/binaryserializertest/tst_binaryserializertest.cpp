#include <QtTest>
#include <QCoreApplication>

#include <util/serializer/binaryserializer.h>

class UnsupportedSerializable: public Serializable
{
public:
    virtual ~UnsupportedSerializable(){}
};

class BinarySerializerTest : public QObject
{
    Q_OBJECT

public:
    BinarySerializerTest() {}
    ~BinarySerializerTest() {}

private slots:
    void serializeTest();

    void libraryTest();
    void schemeTest();
    void gridTest();
    void architectureTest();
};

void BinarySerializerTest::serializeTest()
{
    BinarySerializer serializer;

    Library* l = new Library("test_id", 10.0);
    serializer.serialize(l);
    delete l;

    UnsupportedSerializable* d = new UnsupportedSerializable();
    QVERIFY_EXCEPTION_THROWN(serializer.serialize(d), IllegalArgumentException);
    delete d;
}

void BinarySerializerTest::libraryTest()
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

    BinarySerializer serializer;

    QByteArray arr = serializer.serialize(l);
    Library* deserializedLibrary = static_cast<Library*>(serializer.deserialize(arr));

    QVERIFY(deserializedLibrary->getId() == l->getId());
    QVERIFY(deserializedLibrary->getName() == l->getName());
    QVERIFY(deserializedLibrary->getVersion() == l->getVersion());
    QVERIFY(deserializedLibrary->getElements().size() == l->getElements().size());

    LibraryElement deserializedElement(deserializedLibrary->getElements()[0]);
    QVERIFY(deserializedElement.getId() == el.getId());
    QVERIFY(deserializedElement.getName() == el.getName());
    QVERIFY(deserializedElement.getModel() == el.getModel());
    QVERIFY(deserializedElement.getWidth() == el.getWidth());
    QVERIFY(deserializedElement.getHeight() == el.getHeight());
    QVERIFY(deserializedElement.getPins().size() == el.getPins().size());

    Pin deserializedPin(deserializedElement.getPins()[0]);
    QVERIFY(deserializedPin.getId() == pin.getId());
    QVERIFY(deserializedPin.getX() == pin.getX());
    QVERIFY(deserializedPin.getY() == pin.getY());
    QVERIFY(deserializedPin.getType() == pin.getType());

    delete l;
    delete deserializedLibrary;
}

void BinarySerializerTest::schemeTest()
{
    Scheme* s = new Scheme();

    SchemeElement el1("library id", "aoi43242", Q_INT64_C(9223372036854775807));
    el1.setAlias("FV2");

    SchemeElement el2("23123", "NOR", 2);
    s->getElements().append(el1);
    s->getElements().append(el2);

    Wire w(Q_INT64_C(9223372036854775807), "a", Q_INT64_C(2), "z", WireType::Outer, Q_INT64_C(0));
    s->getWires().append(w);

    BinarySerializer serializer;
    QByteArray array = serializer.serialize(s);
    Scheme* deserializedScheme = static_cast<Scheme*>(serializer.deserialize(array));

    QVERIFY(deserializedScheme->getElements().size() == s->getElements().size());
    QVERIFY(deserializedScheme->getWires().size() == s->getWires().size());

    SchemeElement deserializedElement(deserializedScheme->getElements()[0]);
    QVERIFY(deserializedElement.getElementId() == el1.getElementId());
    QVERIFY(deserializedElement.getLibraryId() == el1.getLibraryId());
    QVERIFY(deserializedElement.getIndex() == el1.getIndex());
    QVERIFY(deserializedElement.getAlias() == el1.getAlias());

    Wire deserializedWire(deserializedScheme->getWires()[0]);
    QVERIFY(deserializedWire.getSrcIndex() == w.getSrcIndex());
    QVERIFY(deserializedWire.getSrcPinId() == w.getSrcPinId());
    QVERIFY(deserializedWire.getDestIndex() == w.getDestIndex());
    QVERIFY(deserializedWire.getDestPinId() == w.getDestPinId());
    QVERIFY(deserializedWire.getIndex() == w.getIndex());
    QVERIFY(deserializedWire.getType() == w.getType());

    delete s;
    delete deserializedScheme;
}

void BinarySerializerTest::gridTest()
{
    Grid* g = new Grid();

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

    BinarySerializer serializer;
    QByteArray array = serializer.serialize(g);
    Grid* deserializedGrid = static_cast<Grid*>(serializer.deserialize(array));

    QVERIFY(deserializedGrid->getRoutedWires() == g->getRoutedWires());
    QVERIFY(deserializedGrid->getRoutedWires()[2] == g->getRoutedWires()[2]);

    QVERIFY(deserializedGrid->getCells().size() == g->getCells().size());
    QVERIFY(deserializedGrid->getCells()[0][0].getType() == g->getCells()[0][0].getType());
    QVERIFY(deserializedGrid->getCells()[0][1].getType() == g->getCells()[0][1].getType());

    QVERIFY(deserializedGrid->getCells()[1][0].getType() == g->getCells()[1][0].getType());
    QVERIFY(deserializedGrid->getCells()[1][0].getIndex() == g->getCells()[1][0].getIndex());

    QVERIFY(deserializedGrid->getCells()[1][1].getType() == g->getCells()[1][1].getType());
    QVERIFY(deserializedGrid->getCells()[1][1].getIndex() == g->getCells()[1][1].getIndex());
    QVERIFY(deserializedGrid->getCells()[1][1].getPinId() == g->getCells()[1][1].getPinId());

    delete g;
    delete deserializedGrid;
}

void BinarySerializerTest::architectureTest()
{
    Architecture* a = new Architecture(DistributionType::Default);
    a->getModel().append(1);
    a->getModel().append(2);
    a->getModel().append(4);

    BinarySerializer serializer;
    QByteArray array = serializer.serialize(a);
    Architecture* deserializedArchitecture = static_cast<Architecture*>(serializer.deserialize(array));

    QVERIFY(deserializedArchitecture->getDistributionType() == a->getDistributionType());
    QVERIFY(deserializedArchitecture->getModel().size() == a->getModel().size());

    QVERIFY(deserializedArchitecture->getModel()[1] == a->getModel()[1]);

    delete a;
    delete deserializedArchitecture;
}

QTEST_MAIN(BinarySerializerTest)

#include "tst_binaryserializertest.moc"
