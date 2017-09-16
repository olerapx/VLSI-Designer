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
    void innerWirePriorityTest();
    void innerWireAndMinConnectionsPriorityTest();
};

void SerialDecompositionTest::decompositionTest()
{
    Scheme* s = new Scheme();

    s->getUsedLibraries().append(QPair<QString, double>("generic", 1.4));
    s->getUsedLibraries().append(QPair<QString, double>("another", 0.2));

    for(int i=0; i<7; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 1, "d", WireType::Inner, 0));

    s->getWires().append(Wire(1, "s", 2, "d", WireType::Outer, 1));
    s->getWires().append(Wire(1, "s", 3, "d", WireType::Outer, 2));

    s->getWires().append(Wire(2, "s", 5, "d", WireType::Outer, 3));
    s->getWires().append(Wire(3, "s", 2, "d", WireType::Outer, 4));

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

    s->getWires().append(Wire(0, "s", 1, "d", WireType::Inner, 0));

    QVERIFY_EXCEPTION_THROWN(decomposition.setParameters(s, 4), IllegalArgumentException);

    delete s;
}

void SerialDecompositionTest::minConnectionsPriorityTest()
{
    Scheme* s = new Scheme();

    for(int i=0; i<4; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 3, "d", WireType::Outer, 1));

    SerialDecomposition decomposition(s, 2);
    QList<Scheme*> list = decomposition.execute();

    QVERIFY(list[0]->getElements()[0] == s->getElements()[1]);
    QVERIFY(list[0]->getElements()[1] == s->getElements()[2]);

    for(Scheme* scheme: list)
        delete scheme;

    delete s;
}

void SerialDecompositionTest::innerWirePriorityTest()
{
    Scheme* s = new Scheme();

    for(int i=0; i<4; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 2, "d", WireType::Inner, 0));
    s->getWires().append(Wire(0, "s", 3, "d", WireType::Outer, 1));

    s->getWires().append(Wire(1, "s", 2, "d", WireType::Outer, 2));
    s->getWires().append(Wire(1, "s", 3, "d", WireType::Outer, 3));
    s->getWires().append(Wire(1, "s", 3, "d", WireType::Outer, 4));

    s->getWires().append(Wire(2, "s", 3, "d", WireType::Outer, 5));

    SerialDecomposition decomposition(s, 2);
    QList<Scheme*> list = decomposition.execute();

    QVERIFY(list[0]->getElements()[0] == s->getElements()[0]);
    QVERIFY(list[0]->getElements()[1] == s->getElements()[2]);

    for(Scheme* scheme: list)
        delete scheme;

    delete s;
}

void SerialDecompositionTest::innerWireAndMinConnectionsPriorityTest()
{
    Scheme* s = new Scheme();

    for(int i=0; i<5; i++)
        s->getElements().append(SchemeElement("lib", QString("el%1").arg(QString::number(i)), i));

    s->getWires().append(Wire(0, "s", 1, "d", WireType::Inner, 0));
    s->getWires().append(Wire(0, "s", 2, "d", WireType::Inner, 1));

    s->getWires().append(Wire(1, "s", 2, "d1", WireType::Outer, 2));
    s->getWires().append(Wire(1, "s", 2, "d2", WireType::Outer, 3));

    s->getWires().append(Wire(2, "s", 3, "d", WireType::Outer, 4));

    s->getWires().append(Wire(3, "s", 4, "d1", WireType::Outer, 5));
    s->getWires().append(Wire(3, "s", 4, "d2", WireType::Outer, 6));
    s->getWires().append(Wire(3, "s", 4, "d3", WireType::Outer, 7));

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
