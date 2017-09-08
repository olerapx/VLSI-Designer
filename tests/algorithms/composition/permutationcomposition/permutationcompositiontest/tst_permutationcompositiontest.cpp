#include <QtTest>
#include <QCoreApplication>

#include <algorithms/composition/permutationcomposition/permutationcomposition.h>
#include <util/renderer/gridrenderer.h>
class PermutationCompositionTest : public QObject
{
    Q_OBJECT

public:
    PermutationCompositionTest() {}
    ~PermutationCompositionTest() {}

private slots:
    void compositionTest();
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

    PermutationComposition composition(grids);
    Grid* grid = composition.execute();

    QVERIFY(grid->getCells().size() == 9);
    QVERIFY(grid->getCells()[0].size() == 8);

    QVERIFY(grid->getRoutedWires().size() == 0);

    QPoint actualPositions[] = { QPoint(0, 0), QPoint(4, 0), QPoint(0, 3), QPoint(4, 3), QPoint(2, 6) };

    for(int i=0; i<grids.size(); i++)
    {
        Grid* g = grids[i];
        QPoint position = actualPositions[i];

        for(int j=0; j<g->getCells().size(); j++)
        {
            for(int k=0; k<g->getCells()[j].size(); k++)
            {
                QVERIFY(grid->getCells()[position.y() + j][position.x() + k] ==
                        g->getCells()[j][k]);
            }
        }
    }

    for(Grid* g: grids)
        delete g;

    delete grid;
}

QTEST_MAIN(PermutationCompositionTest)

#include "tst_permutationcompositiontest.moc"
