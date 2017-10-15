#include <QtTest>
#include <QCoreApplication>

#include <util/misc/gridutils.h>

class GridUtilsTest : public QObject
{
    Q_OBJECT

public:
    ~GridUtilsTest() {}

private slots:
    void cutTest();
    void pasteTest();
    void insertEmptyColumnsTest();
    void removeColumnsTest();
    void rotate90Test();
};

void GridUtilsTest::cutTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Element, 0), Cell(CellType::Pin, 0 , "z"),
          Cell(CellType::DL), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::UD), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::DR), Cell(CellType::LR),
          Cell(CellType::UL), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    for(QList<Cell> list: cells)
        g->getCells().append(list);

    QList<QList<Cell>> list = GridUtils::cut(g, QPoint(1, 1), 2, 3);
    QVERIFY(list.size() == 3);
    QVERIFY(list[0].size() == 2);

    QVERIFY(list[0][1] == Cell(CellType::Pin, 0 , "z"));
    QVERIFY(list[2][1] == Cell(CellType::LR));

    delete g;
}

void GridUtilsTest::pasteTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    for(QList<Cell> list: cells)
        g->getCells().append(list);

    QList<QList<Cell>> data =
    {
        { Cell(CellType::DL), Cell(CellType::UD) },
        { Cell(CellType::UL), Cell(CellType::UDL) }
    };

    GridUtils::paste(g, data, QPoint(1, 1));

    QVERIFY(g->getCells()[1][1] == Cell(CellType::DL));
    QVERIFY(g->getCells()[1][2] == Cell(CellType::UD));
    QVERIFY(g->getCells()[2][1] == Cell(CellType::UL));
    QVERIFY(g->getCells()[2][2] == Cell(CellType::UDL));

    delete g;
}

void GridUtilsTest::insertEmptyColumnsTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::UD), Cell(CellType::Empty), Cell(CellType::LR) },
        { Cell(CellType::UD), Cell(CellType::Empty), Cell(CellType::LR) },
        { Cell(CellType::UDL), Cell(CellType::Empty), Cell(CellType::LRU) }
    };

    for(QList<Cell> list: cells)
        g->getCells().append(list);

    GridUtils::insertEmptyArea(g, QPoint(1, 0), 1, 2);

    QVERIFY(g->getCells()[0][0] == Cell(CellType::UD));
    QVERIFY(g->getCells()[0][1] == Cell(CellType::Empty));
    QVERIFY(g->getCells()[0][2] == Cell(CellType::Empty));
    QVERIFY(g->getCells()[0][3] == Cell(CellType::LR));

    QVERIFY(g->getCells()[1][0] == Cell(CellType::UD));
    QVERIFY(g->getCells()[1][1] == Cell(CellType::Empty));
    QVERIFY(g->getCells()[1][2] == Cell(CellType::Empty));
    QVERIFY(g->getCells()[1][3] == Cell(CellType::LR));

    QVERIFY(g->getCells()[2][0] == Cell(CellType::UDL));
    QVERIFY(g->getCells()[2][1] == Cell(CellType::Empty));
    QVERIFY(g->getCells()[2][2] == Cell(CellType::LRU));

    delete g;
}

void GridUtilsTest::removeColumnsTest()
{
     Grid* g = new Grid();

     QList<QList<Cell>> cells =
     {
         { Cell(CellType::UD), Cell(CellType::Empty), Cell(CellType::LR) },
         { Cell(CellType::UD), Cell(CellType::Empty), Cell(CellType::LR) },
         { Cell(CellType::UDL), Cell(CellType::Empty), Cell(CellType::LRU) }
     };

     for(QList<Cell> list: cells)
         g->getCells().append(list);

     GridUtils::removeArea(g, QPoint(1, 0), 1, 2);

    QVERIFY(g->getCells()[0][0] == Cell(CellType::UD));
    QVERIFY(g->getCells()[0][1] == Cell(CellType::LR));

    QVERIFY(g->getCells()[1][0] == Cell(CellType::UD));
    QVERIFY(g->getCells()[1][1] == Cell(CellType::LR));

    QVERIFY(g->getCells()[2][0] == Cell(CellType::UDL));
    QVERIFY(g->getCells()[2][1] == Cell(CellType::Empty));
    QVERIFY(g->getCells()[2][2] == Cell(CellType::LRU));

    delete g;
}

void GridUtilsTest::rotate90Test()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 0, "p1"), Cell(CellType::Empty),
          Cell(CellType::UR), Cell(CellType::UL), Cell(CellType::DL), Cell(CellType::DR) },

        { Cell(CellType::Empty), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Empty),
          Cell(CellType::UDLR), Cell(CellType::UDLRI), Cell(CellType::UDL), Cell(CellType::LRU) },

        { Cell(CellType::Pin, 0, "p2"), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Empty),
          Cell(CellType::UDR), Cell(CellType::LRD), Cell(CellType::UD), Cell(CellType::LR) }
    };

    QList<RoutedWireIndex> routedWires = { 0, 10, 12 };
    QList<WireData> wiresData = { WireData(0, QPoint(5, 2), QPoint(2, 1), WirePosition::Internal),
                                  WireData(0, QPoint(6, 1), QPoint(0, 0), WirePosition::External) };

    g->getCells().append(cells);
    g->getRoutedWires().append(routedWires);
    g->getWiresData().append(wiresData);

    Grid* res = GridUtils::rotate90(g);

    QVERIFY(res->getHeight() == 8);
    QVERIFY(res->getWidth() == 3);

    QList<QList<Cell>> newCells =
    {
        { Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty), Cell(CellType::Empty) },
        { Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Empty) },
        { Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p1") },
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },
        { Cell(CellType::LRD), Cell(CellType::UDLR), Cell(CellType::DR) },
        { Cell(CellType::UDL), Cell(CellType::UDLRI), Cell(CellType::UR) },
        { Cell(CellType::LR), Cell(CellType::LRU), Cell(CellType::UL) },
        { Cell(CellType::UD), Cell(CellType::UDR), Cell(CellType::DL) }
    };

    QVERIFY(res->getCells()== newCells);

    QVERIFY(res->getRoutedWires() == routedWires);

    QVERIFY(res->getWiresData()[0] == WireData(0, QPoint(0, 5), QPoint(1, 2), WirePosition::Internal));
    QVERIFY(res->getWiresData()[1] == WireData(0, QPoint(1, 6), QPoint(0, 0), WirePosition::External));

    for(int i=0; i<3; i++)
    {
        Grid* newRes = GridUtils::rotate90(res);
        delete res;
        res = newRes;
    }

    QVERIFY(*res == *g);

    delete g;
    delete res;
}

QTEST_MAIN(GridUtilsTest)

#include "tst_gridutilstest.moc"
