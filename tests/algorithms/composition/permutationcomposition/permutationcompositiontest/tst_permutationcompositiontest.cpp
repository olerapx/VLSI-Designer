#include <QtTest>
#include <QCoreApplication>

#include <algorithms/composition/permutationcomposition/permutationcomposition.h>

class PermutationCompositionTest : public QObject
{
    Q_OBJECT

public:
    PermutationCompositionTest() {}
    ~PermutationCompositionTest() {}

private slots:
    void compositionTest();
    void verifyGridPartsOnPositions(QList<Grid*>& grids, QPoint actualPositions[], Grid* grid);
    void wiresDataCompositionTest();
    void optimizationTest();

    static bool wiresDataIndexComparator(WireData& first, WireData& second) { return first.getIndex() < second.getIndex(); }
};

void PermutationCompositionTest::compositionTest()
{
    QList<Grid*> grids;
    for(int i=0; i<5; i++)
        grids.append(new Grid());

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Empty) },
        { Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p1") },
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty) }
    };

    grids[0]->getCells().append(cells);

    cells =
    {
        { Cell(CellType::Empty), Cell(CellType::UD), Cell(CellType::Empty) },
        { Cell(CellType::LR), Cell(CellType::UDLRI), Cell(CellType::LR) },
        { Cell(CellType::Empty), Cell(CellType::UD), Cell(CellType::Empty) }
    };

    grids[1]->getCells().append(cells);

    cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1") },
        { Cell(CellType::Pin, 1, "p2"), Cell(CellType::Element, 1) },
        { Cell(CellType::Empty), Cell(CellType::Pin, 1, "p2") }
    };

    grids[2]->getCells().append(cells);

    cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::UD), Cell(CellType::Empty) },
        { Cell(CellType::Element, 2), Cell(CellType::Pin, 2, "p1"), Cell(CellType::LRU), Cell(CellType::LR) }
    };

    grids[3]->getCells().append(cells);

    cells =
    {
        { Cell(CellType::Pin, 3, "p1"), Cell(CellType::Element, 3), Cell(CellType::Pin, 3, "p2") }
    };

    grids[4]->getCells().append(cells);

    grids[0]->getRoutedWires().append({ 0, 1, 4 });
    grids[2]->getRoutedWires().append({ 5, 8 });

    Scheme* s = new Scheme();

    PermutationComposition composition(grids, s);
    Grid* grid = composition.execute();

    QVERIFY(grid->getHeight() == 12);
    QVERIFY(grid->getWidth() == 8);

    QVERIFY(grid->getWiresData().size() == 0);
    QVERIFY(grid->getRoutedWires().size() == 5);

    QPoint actualPositions[] = { QPoint(0, 0), QPoint(4, 0), QPoint(1, 3), QPoint(4, 3), QPoint(2, 7) };

    verifyGridPartsOnPositions(grids, actualPositions, grid);

    QVERIFY(QSize(grids[0]->getHeight(), grids[0]->getWidth()) == QSize(3, 4));
    QVERIFY(QSize(grids[1]->getHeight(), grids[1]->getWidth()) == QSize(3, 3));
    QVERIFY(QSize(grids[2]->getHeight(), grids[2]->getWidth()) == QSize(3, 2));
    QVERIFY(QSize(grids[3]->getHeight(), grids[3]->getWidth()) == QSize(2, 4));
    QVERIFY(QSize(grids[4]->getHeight(), grids[4]->getWidth()) == QSize(1, 3));

    for(Grid* g: grids)
        delete g;

    delete s;
    delete grid;    
}

void PermutationCompositionTest::verifyGridPartsOnPositions(QList<Grid*>& grids, QPoint actualPositions[], Grid* grid)
{
    for(int i=0; i<grids.size(); i++)
    {
        Grid* g = grids[i];
        QPoint position = actualPositions[i];

        for(int j=0; j<g->getHeight(); j++)
        {
            for(int k=0; k<g->getWidth(); k++)
            {
                QVERIFY(grid->getCells()[position.y() + j][position.x() + k] ==
                        g->getCells()[j][k]);
            }
        }
    }
}

void PermutationCompositionTest::wiresDataCompositionTest()
{
    QList<Grid*> grids;
    for(int i=0; i<3; i++)
        grids.append(new Grid());

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Pin, 0, "p1"), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"), Cell(CellType::Element, 1) },
        { Cell(CellType::Element, 0), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Element, 1) },
        { Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p2") }
    };

    grids[0]->getCells().append(cells);

    grids[0]->getWiresData().append(WireData(0, QPoint(0, 0), QPoint(0, 0), WirePosition::External));
    grids[0]->getWiresData().append(WireData(1, QPoint(2, 0), QPoint(0, 0), WirePosition::External));
    grids[0]->getWiresData().append(WireData(3, QPoint(3, 2), QPoint(1, 2), WirePosition::Internal));

    cells =
    {
        { Cell(CellType::Element, 2), Cell(CellType::Pin, 2, "p1") },
        { Cell(CellType::Pin, 2, "p2"), Cell(CellType::Pin, 3, "p1") },
        { Cell(CellType::Pin, 3, "p2"), Cell(CellType::Element, 3) }
    };

    grids[1]->getCells().append(cells);

    grids[1]->getWiresData().append(WireData(0, QPoint(1, 0), QPoint(0, 0), WirePosition::External));
    grids[1]->getWiresData().append(WireData(2, QPoint(0, 1), QPoint(0, 0), WirePosition::External));
    grids[1]->getWiresData().append(WireData(4, QPoint(1, 1), QPoint(0, 2), WirePosition::Internal));

    cells =
    {
        { Cell(CellType::Element, 4) },
        { Cell(CellType::Pin, 4, "p1") }
    };

    grids[2]->getCells().append(cells);

    grids[2]->getWiresData().append(WireData(5, QPoint(0, 1), QPoint(0, 0), WirePosition::External));

    Scheme* s = new Scheme();
    s->getWires().append(Wire(0, "p1", 2, "p1", WireType::Inner, 0));
    s->getWires().append(Wire(1, "p1", 10, "p4", WireType::Outer, 1));
    s->getWires().append(Wire(2, "p2", 10, "p5", WireType::Outer, 2));
    s->getWires().append(Wire(0, "p2", 1, "p2", WireType::Inner, 3));
    s->getWires().append(Wire(3, "p1", 3, "p2", WireType::Outer, 4));
    s->getWires().append(Wire(4, "p1", 11, "p3", WireType::Outer, 5));

    PermutationComposition composition(grids, s);
    Grid* grid = composition.execute();

    QVERIFY(grid->getWiresData().size() == 4);

    std::sort(grid->getWiresData().begin(), grid->getWiresData().end(), PermutationCompositionTest::wiresDataIndexComparator);

    WireData data = grid->getWiresData()[0];
    QVERIFY(data.getWirePosition() == WirePosition::Internal);
    QVERIFY(data.getSrcCoord() == QPoint(4, 0));
    QVERIFY(data.getDestCoord() == QPoint(2, 0));

    data = grid->getWiresData()[1];
    QVERIFY(data.getWirePosition() == WirePosition::External);
    QVERIFY(data.getSrcCoord() == QPoint(6, 0));

    data = grid->getWiresData()[2];
    QVERIFY(data.getWirePosition() == WirePosition::External);
    QVERIFY(data.getSrcCoord() == QPoint(1, 1));

    data = grid->getWiresData()[3];
    QVERIFY(data.getWirePosition() == WirePosition::Internal);
    QVERIFY(data.getSrcCoord() == QPoint(7, 2));
    QVERIFY(data.getDestCoord() == QPoint(5, 2));

    data = grid->getWiresData()[4];
    QVERIFY(data.getWirePosition() == WirePosition::Internal);
    QVERIFY(data.getSrcCoord() == QPoint(2, 1));
    QVERIFY(data.getDestCoord() == QPoint(1, 2));

    data = grid->getWiresData()[5];
    QVERIFY(data.getWirePosition() == WirePosition::External);
    QVERIFY(data.getSrcCoord() == QPoint(3, 4));

    for(Grid* g: grids)
        delete g;

    delete s;
    delete grid;
}

void PermutationCompositionTest::optimizationTest()
{
    QList<Grid*> grids;
    for(int i=0; i<4; i++)
        grids.append(new Grid());

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Pin, 0, "p1"), Cell(CellType::Empty) },
        { Cell(CellType::Empty), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p2") }
    };

    grids[0]->getCells().append(cells);

    grids[0]->getWiresData().append(WireData(0, QPoint(1, 0), QPoint(0, 0), WirePosition::External));
    grids[0]->getWiresData().append(WireData(1, QPoint(2, 1), QPoint(0, 0), WirePosition::External));

    cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty) },
        { Cell(CellType::Pin, 1, "p1"), Cell(CellType::Element, 1) }
    };

    grids[1]->getCells().append(cells);

    grids[1]->getWiresData().append(WireData(0, QPoint(0, 1), QPoint(0, 0), WirePosition::External));

    cells =
    {
        { Cell(CellType::UDLR) },
        { Cell(CellType::UDLRI) }
    };

    grids[2]->getCells().append(cells);

    cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },
        { Cell(CellType::Pin, 2, "p1"), Cell(CellType::Element, 2), Cell(CellType::Empty) }
    };

    grids[3]->getCells().append(cells);

    grids[3]->getWiresData().append(WireData(1, QPoint(0, 1), QPoint(0, 0), WirePosition::External));

    Scheme* s = new Scheme();
    s->getWires().append(Wire(0, "p1", 1, "p1", WireType::Outer, 0));
    s->getWires().append(Wire(0, "p2", 2, "p1", WireType::Inner, 1));

    PermutationComposition composition(grids, s);
    Grid* grid = composition.execute();

    QPoint actualPositions[] = { QPoint(0, 2), QPoint(0, 0), QPoint(4, 0), QPoint(3, 2) };

    verifyGridPartsOnPositions(grids, actualPositions, grid);

    for(Grid* g: grids)
        delete g;

    delete s;
    delete grid;
}

QTEST_MAIN(PermutationCompositionTest)

#include "tst_permutationcompositiontest.moc"
