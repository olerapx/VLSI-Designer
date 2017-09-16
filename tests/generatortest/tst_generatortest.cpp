#include <QtTest>
#include <QCoreApplication>

#include <generator/generator.h>
#include <util/serializer/jsonserializer.h>

class GeneratorTest : public QObject
{
    Q_OBJECT

public:
    GeneratorTest();
    ~GeneratorTest() {}

private slots:
    void generateTest();
    void distributionTest();
    void outerIfUnableInnerTest();
    void changeBranchingTest();

private:
    QString datamodelsDir;
    QList<Library*> libraries;

    void readLibraries();

    bool containsOuter(Scheme* s);

    Scheme* scheme;
};

GeneratorTest::GeneratorTest()
{
    QDir dir (QApplication::applicationDirPath());
    dir.cdUp();
    datamodelsDir = dir.absolutePath();

    readLibraries();
}

void GeneratorTest::readLibraries()
{
    libraries.clear();

    QFile f(datamodelsDir+"/datamodels/library_generator.json");
    f.open(QIODevice::ReadOnly);
    JsonSerializer json;
    libraries.append(static_cast<Library*>(json.deserialize(f.readAll())));
    f.close();
}

void GeneratorTest::generateTest()
{
    readLibraries();
    libraries.at(0)->getElements().removeAt(1);
    GeneratorParameters param(libraries);

    param.setElementsNumber(5);

    Generator* g = new Generator(param);
    Scheme* s = g->execute();

    QVERIFY(s->getUsedLibraries().size() == 1);
    QVERIFY(s->getUsedLibraries()[0].first == "generic");
    QVERIFY(s->getUsedLibraries()[0].second == 0.1);

    for(int i=0; i<s->getWires().size(); i++)
    {
        Wire w = s->getWires()[i];
        QString srcPinId = w.getSrcPinId();
        QString destPinId = w.getDestPinId();

        for(Pin p: libraries[0]->getElements()[0].getPins())
        {
            if(p.getId() == srcPinId)
                QVERIFY(p.getType() == PinType::Output);
            else if (p.getId() == destPinId)
                QVERIFY(p.getType() == PinType::Input);
        }

        if(i != 0)
            QVERIFY(w.getIndex() - s->getWires()[i-1].getIndex() == 1);
    }

    for(int i=1; i<s->getElements().size(); i++)
        QVERIFY(s->getElements()[i].getIndex() - s->getElements()[i-1].getIndex() == 1);

    for(int i=0; i<s->getWires().size(); i++)
    {
        qint64 destIndex = s->getWires()[i].getDestIndex();
        QString destPinId = s->getWires()[i].getDestPinId();

        for(int j=0; j<s->getWires().size(); j++)
        {
            if(i == j) continue;

            QVERIFY(!(destIndex == s->getWires()[j].getDestIndex() && destPinId == s->getWires()[j].getDestPinId()));
        }
    }

    delete s;
    delete g;
}

void GeneratorTest::distributionTest()
{
    readLibraries();
    GeneratorParameters param(libraries);

    param.setElementsNumber(17);

    param.setNodeCapacity(2, 0.1, 2, 2);
    param.setBranching(2, 0.2, 2, 2);

    Generator* g = new Generator(param);
    Scheme* s = g->execute();

    QVERIFY(s->getElements().size() == 17);
    QVERIFY(s->getWires().size() == 17*2);

    for(int i=0; i<16; i+=pow(2, 2))
    {
        QVERIFY(LibraryUtils::getCorrespondingElement(s->getElements()[i], libraries) == LibraryUtils::getCorrespondingElement(s->getElements()[i+1], libraries));
        QVERIFY(LibraryUtils::getCorrespondingElement(s->getElements()[i], libraries) == LibraryUtils::getCorrespondingElement(s->getElements()[i+2], libraries));
        QVERIFY(LibraryUtils::getCorrespondingElement(s->getElements()[i], libraries) == LibraryUtils::getCorrespondingElement(s->getElements()[i+3], libraries));
    }

    delete s;
    delete g;
}

void GeneratorTest::outerIfUnableInnerTest()
{
    readLibraries();
    libraries.at(0)->getElements().removeAt(1);
    GeneratorParameters param(libraries);

    param.setElementsNumber(5);

    param.setNodeCapacity(1, 0.1, 1, 1);
    param.setBranching(3, 0.2, 3, 3);
    param.setInnerWireChance(1);

    Generator* g = new Generator(param);
    Scheme* s = g->execute();

    QVERIFY(containsOuter(s));

    delete s;
    delete g;
}

bool GeneratorTest::containsOuter(Scheme *s)
{
    for(Wire w: s->getWires())
        if(w.getType() == WireType::Outer)
            return true;

    return false;
}

void GeneratorTest::changeBranchingTest()
{
    readLibraries();
    libraries.at(0)->getElements().removeAt(1);
    GeneratorParameters param(libraries);

    param.setElementsNumber(3);

    param.setNodeCapacity(1, 0.1, 1, 1);
    param.setBranching(7, 0.2, 7, 7);

    Generator* g = new Generator(param);
    Scheme* s = g->execute();

    QVERIFY(g->getParameters().getBranchingMean() == 2);
    QVERIFY(g->getParameters().getBranchingLowerLimit() == 2);
    QVERIFY(g->getParameters().getBranchingUpperLimit() == 2);

    delete s;
    delete g;
}

QTEST_MAIN(GeneratorTest)

#include "tst_generatortest.moc"
