#include <QtTest>
#include <QCoreApplication>

#include <util/aliaser/aliaser.h>

class AliaserTest : public QObject
{
    Q_OBJECT

public:
    AliaserTest() {}
    ~AliaserTest() {}

private slots:
    void generateTest();
    void repeatingTypesTest();
};

void AliaserTest::generateTest()
{
    Aliaser aliaser(2);

    Scheme* s = new Scheme();

    int count = pow((26 + 26 + 10), 2) + 1;

    for(int i=0; i<count; i++)
        s->getElements().append(SchemeElement("lib", "el" + QString::number(i), 1));

    aliaser.execute(s);
    QVERIFY(s->getElements()[0].getAlias() == "AA");
    QVERIFY(s->getElements()[s->getElements().size()-2].getAlias() == "99");
    QVERIFY(s->getElements()[s->getElements().size()-1].getAlias().isEmpty());

    delete s;
}

void AliaserTest::repeatingTypesTest()
{
    Aliaser aliaser(1);

    Scheme* s = new Scheme();

    s->getElements().append(SchemeElement("lib", "el1", 1));
    s->getElements().append(SchemeElement("lib", "el2", 2));
    s->getElements().append(SchemeElement("lib", "el2", 3));
    s->getElements().append(SchemeElement("lib", "el1", 4));

    aliaser.execute(s);

    QVERIFY(s->getElements()[0].getAlias() == "A");
    QVERIFY(s->getElements()[1].getAlias() == "B");
    QVERIFY(s->getElements()[2].getAlias() == "B");
    QVERIFY(s->getElements()[3].getAlias() == "A");

    delete s;
}

QTEST_MAIN(AliaserTest)

#include "tst_aliasertest.moc"
