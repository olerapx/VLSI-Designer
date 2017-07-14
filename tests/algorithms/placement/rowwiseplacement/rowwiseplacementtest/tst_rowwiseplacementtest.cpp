#include <QtTest>
#include <QCoreApplication>

#include <algorithms/placement/rowwiseplacement/rowwiseplacement.h>
#include <generator/generator.h>
#include <util/renderer/gridrenderer.h>
#include <util/serializer/jsonserializer.h>

class RowWisePlacementTest : public QObject
{
    Q_OBJECT

public:
    RowWisePlacementTest() {}
    ~RowWisePlacementTest() {}

private slots:
    void placementTest();
};

void RowWisePlacementTest::placementTest()
{
    QFile f("../../../../../libraries/generic.json");
    f.open(QIODevice::ReadOnly);
    JsonSerializer j;
    QList<Library*> list;
    list.append((Library*) j.deserialize(f.readAll()));
    f.close();

    GeneratorParameters param(list);
    param.setElementsNumber(100);
    param.setNodeCapacity(2, 0.1, 1, 3);

    Generator generator(param);
    Scheme* s = generator.execute();

    RowWisePlacement placement(s, list, 1.5);
    Grid* g = placement.execute()->getGrid();

    GridRenderer renderer(g, s);
    QImage img = renderer.execute();

    img.save("GRID.png");
}

QTEST_MAIN(RowWisePlacementTest)

#include "tst_rowwiseplacementtest.moc"
