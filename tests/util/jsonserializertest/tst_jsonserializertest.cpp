#include <QtTest>
#include <QCoreApplication>

#include <util/jsonserializer.h>

class JsonSerializerTest : public QObject
{
    Q_OBJECT

public:
    JsonSerializerTest();
    ~JsonSerializerTest();

private slots:
    void deserializeLibraryTest();
};

JsonSerializerTest::JsonSerializerTest()
{

}

JsonSerializerTest::~JsonSerializerTest()
{

}

void JsonSerializerTest::deserializeLibraryTest()
{
    JsonSerializer json;
}


QTEST_MAIN(JsonSerializerTest)

#include "tst_jsonserializertest.moc"
