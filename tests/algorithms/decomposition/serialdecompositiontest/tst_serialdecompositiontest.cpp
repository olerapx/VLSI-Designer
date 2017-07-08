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

private:

};

void SerialDecompositionTest::decompositionTest()
{
    Scheme* s = new Scheme();

    s->getElements().append(SchemeElement("lib", "el1", 0));
    s->getElements().append(SchemeElement("lib", "el2", 1));
    s->getElements().append(SchemeElement("lib", "el3", 2));
    s->getElements().append(SchemeElement("lib", "el4", 3));
    s->getElements().append(SchemeElement("lib", "el5", 4));
    s->getElements().append(SchemeElement("lib", "el6", 5));
    s->getElements().append(SchemeElement("lib", "el7", 6));

    s->getWires().append(Wire(0, "s", 1, "d", WireType::Inner, 0));

    s->getWires().append(Wire(1, "s", 2, "d", WireType::Outer, 1));
    s->getWires().append(Wire(1, "s", 3, "d", WireType::Outer, 2));

    s->getWires().append(Wire(2, "s", 5, "d", WireType::Outer, 2));
    s->getWires().append(Wire(3, "s", 2, "d", WireType::Outer, 2));


    SerialDecomposition decomposition(s, 3);
    QList<Scheme*> list = decomposition.execute();

    QVERIFY(list.size() == 3);
    QVERIFY(list[0]->getElements().size() == 2);
    QVERIFY(list[1]->getElements().size() == 2);
    QVERIFY(list[2]->getElements().size() == 3);

    delete s;
}

QTEST_MAIN(SerialDecompositionTest)

#include "tst_serialdecompositiontest.moc"
