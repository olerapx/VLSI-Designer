#include <QtTest>
#include <QCoreApplication>

#include <algorithms/decomposition/serialdecomposition/serialdecomposition.h>

class SerialDecompositionTest : public QObject
{
    Q_OBJECT

public:
    SerialDecompositionTest() {}
    ~SerialDecompositionTest() {}

private slots:
    void decompositionTest();
    void minConnectionsPriorityTest();
    void inNodeWirePriorityTest();
    void inNodeWireAndMinConnectionsPriorityTest();
};

void SerialDecompositionTest::decompositionTest()
{
    Scheme* s = new Scheme();

    s->getUsedLibraries().append(QPair<QString, Version>("generic", Version(1, 4, 0, 0)));
    s->getUsedLibraries().append(QPair<QString, Version>("another", Version(0, 2, 0, 0)));

    for(int i=0; i<7; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 1, "d", WireType::InNode, 0));

    s->getWires().append(Wire(1, "s", 2, "d", WireType::InterNode, 1));
    s->getWires().append(Wire(1, "s", 3, "d", WireType::InterNode, 2));

    s->getWires().append(Wire(2, "s", 5, "d", WireType::InterNode, 3));
    s->getWires().append(Wire(3, "s", 2, "d", WireType::InterNode, 4));

    SerialDecomposition decomposition(s, 3);
    QList<Scheme*> list = decomposition.execute();

    for(Scheme* scheme: list)
        QVERIFY(scheme->getUsedLibraries() == s->getUsedLibraries());

    QVERIFY(list.size() == 3);
    QVERIFY(list[0]->getElements().size() == 3);
    QVERIFY(list[1]->getElements().size() == 2);
    QVERIFY(list[2]->getElements().size() == 2);
    QVERIFY(list[0]->getWires().size() == 5);

    for(Scheme* scheme: list)
        delete scheme;

    delete s;

    s = new Scheme();

    for(int i=0; i<3; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 1, "d", WireType::InNode, 0));

    QVERIFY_EXCEPTION_THROWN(decomposition.setParameters(s, 4), IllegalArgumentException);

    delete s;
}

void SerialDecompositionTest::minConnectionsPriorityTest()
{
    Scheme* s = new Scheme();

    for(int i=0; i<4; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 3, "d", WireType::InterNode, 1));

    SerialDecomposition decomposition(s, 2);
    QList<Scheme*> list = decomposition.execute();

    QVERIFY(list[0]->getElements()[0] == s->getElements()[1]);
    QVERIFY(list[0]->getElements()[1] == s->getElements()[2]);

    for(Scheme* scheme: list)
        delete scheme;

    delete s;
}

void SerialDecompositionTest::inNodeWirePriorityTest()
{
    Scheme* s = new Scheme();

    for(int i=0; i<4; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 2, "d", WireType::InNode, 0));
    s->getWires().append(Wire(0, "s", 3, "d", WireType::InterNode, 1));

    s->getWires().append(Wire(1, "s", 2, "d", WireType::InterNode, 2));
    s->getWires().append(Wire(1, "s", 3, "d", WireType::InterNode, 3));
    s->getWires().append(Wire(1, "s", 3, "d", WireType::InterNode, 4));

    s->getWires().append(Wire(2, "s", 3, "d", WireType::InterNode, 5));

    SerialDecomposition decomposition(s, 2);
    QList<Scheme*> list = decomposition.execute();

    QVERIFY(list[0]->getElements()[0] == s->getElements()[0]);
    QVERIFY(list[0]->getElements()[1] == s->getElements()[2]);

    for(Scheme* scheme: list)
        delete scheme;

    delete s;
}

void SerialDecompositionTest::inNodeWireAndMinConnectionsPriorityTest()
{
    Scheme* s = new Scheme();

    for(int i=0; i<5; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 1, "d", WireType::InNode, 0));
    s->getWires().append(Wire(0, "s", 2, "d", WireType::InNode, 1));

    s->getWires().append(Wire(1, "s", 2, "d1", WireType::InterNode, 2));
    s->getWires().append(Wire(1, "s", 2, "d2", WireType::InterNode, 3));

    s->getWires().append(Wire(2, "s", 3, "d", WireType::InterNode, 4));

    s->getWires().append(Wire(3, "s", 4, "d1", WireType::InterNode, 5));
    s->getWires().append(Wire(3, "s", 4, "d2", WireType::InterNode, 6));
    s->getWires().append(Wire(3, "s", 4, "d3", WireType::InterNode, 7));

    SerialDecomposition decomposition(s, 2);
    QList<Scheme*> list = decomposition.execute();

    QVERIFY(list[0]->getElements()[0] == s->getElements()[0]);
    QVERIFY(list[0]->getElements()[1] == s->getElements()[1]);
    QVERIFY(list[0]->getElements()[2] == s->getElements()[2]);

    for(Scheme* scheme: list)
        delete scheme;

    delete s;
}

QTEST_MAIN(SerialDecompositionTest)

#include "tst_serialdecompositiontest.moc"
