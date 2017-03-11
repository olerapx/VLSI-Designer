#include <QtTest>
#include <QCoreApplication>

class JsonSerializerTest : public QObject
{
    Q_OBJECT

public:
    JsonSerializerTest();
    ~JsonSerializerTest();

private slots:
    void initTestCase();
    void cleanupTestCase();

};

JsonSerializerTest::JsonSerializerTest()
{

}

JsonSerializerTest::~JsonSerializerTest()
{

}

void JsonSerializerTest::initTestCase()
{

}

void JsonSerializerTest::cleanupTestCase()
{

}

QTEST_MAIN(JsonSerializerTest)

#include "tst_jsonserializertest.moc"
