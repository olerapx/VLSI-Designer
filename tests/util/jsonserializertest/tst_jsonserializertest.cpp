#include <QtTest>
#include <QCoreApplication>
#include <QFile>

#include <util/jsonserializer.h>

class JsonSerializerTest : public QObject
{
    Q_OBJECT

    QString datamodelsDir;

public:
    JsonSerializerTest();
    ~JsonSerializerTest();

private slots:
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

    Element* el = l->getElements().at(0);
    QVERIFY(el->getId()=="an2");
    QVERIFY(el->getModel()=="Generic");
    QVERIFY(el->getHeight() == 4);
    QVERIFY(el->getWidth() == 2);
    QVERIFY(el->getPins().size() == 3);

    Pin* p = el->getPins().at(0);
    QVERIFY(p->getId()=="a");
    QVERIFY(p->getType() == PinType::Input);
    QVERIFY(p->getX()==0);
    QVERIFY(p->getY()==0);

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

    SchemeElement* el = s->getElements().at(0);
    QVERIFY(el->getLibraryId() == "generic");
    QVERIFY(el->getElementId() == "an2");
    QVERIFY(el->getIndex() == 9223372036854775807);

    Wire* w = s->getWires().at(0);
    QVERIFY(w->getSrcIndex() == 9223372036854775807);
    QVERIFY(w->getSrcPinId() == "z");
    QVERIFY(w->getDestIndex() == 1);
    QVERIFY(w->getDestPinId() == "a");
    QVERIFY(w->getType() == WireType::Inner);
    QVERIFY(w->getIndex() == 0);

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

    QList<Cell*> list = g->getCells().at(0);
    QVERIFY(list.size() == 4);

    QList<qint64> routedWires = {0, 1};
    QVERIFY(g->getRoutedWires() == routedWires);
    QVERIFY(g->getInitialLevel() == 1);

    Cell* emptyCell = g->getCells().at(0).at(0);
    QVERIFY(emptyCell->getType() == CellType::Empty);

    Cell* elementCell = g->getCells().at(0).at(1);
    QVERIFY(elementCell->getType() == CellType::Element);
    QVERIFY(elementCell->getIndex() == 0);

    Cell* pinCell = g->getCells().at(0).at(2);
    QVERIFY(pinCell->getType() == CellType::Pin);
    QVERIFY (pinCell->getIndex() == 0);
    QVERIFY(pinCell->getPinId() == "z");

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

    QList<int> model = {2, 3, 2};

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
