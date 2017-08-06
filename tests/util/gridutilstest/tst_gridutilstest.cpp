#include <QtTest>
#include <QCoreApplication>

#include <util/gridutils.h>

class GridUtilsTest : public QObject
{
    Q_OBJECT

public:
    ~GridUtilsTest() {}

private slots:
    void cutTest();
    void pasteTest();
    void insertEmptyAreaTest();
    void removeAreaTest();
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

void GridUtilsTest::insertEmptyAreaTest()
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

void GridUtilsTest::removeAreaTest()
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

QTEST_MAIN(GridUtilsTest)

#include "tst_gridutilstest.moc"
