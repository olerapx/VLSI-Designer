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

    QVERIFY(grid->getHeight() == 9);
    QVERIFY(grid->getWidth() == 8);

    QVERIFY(grid->getRoutedWires().size() == 0);

    QPoint actualPositions[] = { QPoint(0, 0), QPoint(4, 0), QPoint(1, 3), QPoint(4, 3), QPoint(2, 7) };

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

    QVERIFY(QSize(grids[0]->getHeight(), grids[0]->getWidth()) == QSize(3, 4));
    QVERIFY(QSize(grids[1]->getHeight(), grids[1]->getWidth()) == QSize(3, 3));
    QVERIFY(QSize(grids[2]->getHeight(), grids[2]->getWidth()) == QSize(3, 2));
    QVERIFY(QSize(grids[3]->getHeight(), grids[3]->getWidth()) == QSize(2, 4));
    QVERIFY(QSize(grids[4]->getHeight(), grids[4]->getWidth()) == QSize(1, 3));

    for(Grid* g: grids)
        delete g;

    delete grid;
}

QTEST_MAIN(PermutationCompositionTest)

#include "tst_permutationcompositiontest.moc"
