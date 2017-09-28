#include <QtTest>
#include <QCoreApplication>

#include <datamodels/version/version.h>

class VersionTest : public QObject
{
    Q_OBJECT

public:
    VersionTest() {}
    ~VersionTest() {}

private slots:
    void toStringTest();
    void fromStringTest();
    void comparisonTest();
};

void VersionTest::toStringTest()
{
    Version version(1, 2, 31, 4);
    QVERIFY(version.toString() == "1.2.31-4");

    Version version2(1, 2, 3, 0);
    QVERIFY(version2.toString() == "1.2.3");

    Version version3(1, 2, 0, 0);
    QVERIFY(version3.toString() == "1.2");

    Version version4(0, 0, 0, 0);
    QVERIFY(version4.toString() == "0.0");

    Version version5(1, 0, 3, 4);
    QVERIFY(version5.toString() == "1.0.3-4");
}

void VersionTest::fromStringTest()
{
    QString s = "1.2.30-4";
    QVERIFY(Version(s) == Version(1, 2, 30, 4));

    s = "1.20.3";
    QVERIFY(Version(s) == Version(1, 20, 3, 0));

    s = "1.2";
    QVERIFY(Version(s) == Version(1, 2, 0, 0));

    s = "10";
    QVERIFY(Version(s) == Version(10, 0, 0, 0));

    s = "10.0.3-1";
    QVERIFY(Version(s) == Version(10, 0, 3, 1));
}

void VersionTest::comparisonTest()
{
    Version v1(1, 2, 3, 4);
    Version v2(1, 2, 3, 4);

    QVERIFY(v1 == v2);

    v2 = Version(1, 2, 3, 5);
    QVERIFY(v1 < v2);
    QVERIFY(v2 > v1);

    v2 = Version(0, 10, 3, 4);
    QVERIFY(v1 > v2);
    QVERIFY(v2 < v1);
}

QTEST_MAIN(VersionTest)

#include "tst_versiontest.moc"
