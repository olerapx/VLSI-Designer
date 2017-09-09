#include <QtTest>
#include <QCoreApplication>

#include <algorithms/routing/leerouting/leerouting.h>
#include <datamodels/library/library.h>

class LeeRoutingTest : public QObject
{
    Q_OBJECT

public:
    LeeRoutingTest();
    ~LeeRoutingTest() {}

private slots:
    void routingTest();
    void branchingTest();

    void wireSortingTest();
    void excludedWiresTest();

    void extendHorizontallyTest();
    void extendVerticallyTest();

    void extensionIsUnavailableTest();
    void maxExtensionAttemptsReachedTest();

private:
    QList<Library*> libraries;
};

LeeRoutingTest::LeeRoutingTest()
{
    Library* library = new Library("lib", 1.0);

    LibraryElement el1("el1", 4, 3);
    el1.getPins().append(Pin("p1", 0, 1, PinType::Input));
    el1.getPins().append(Pin("p2", 0, 2, PinType::Input));
    el1.getPins().append(Pin("p3", 2, 2, PinType::Output));

    LibraryElement el2("el2", 4, 4);
    el2.getPins().append(Pin("p1", 0, 1, PinType::Input));
    el2.getPins().append(Pin("p2", 0, 2, PinType::Input));
    el2.getPins().append(Pin("p3", 3, 2, PinType::Output));

    LibraryElement el3("el3", 5, 4);
    el3.getPins().append(Pin("p1", 0, 1, PinType::Input));
    el3.getPins().append(Pin("p2", 0, 2, PinType::Input));
    el3.getPins().append(Pin("p3", 0, 3, PinType::Input));
    el3.getPins().append(Pin("p4", 2, 0, PinType::Input));
    el3.getPins().append(Pin("p5", 3, 3, PinType::Output));

    LibraryElement el4("el4", 3, 4);
    el4.getPins().append(Pin("p1", 0, 1, PinType::Input));
    el4.getPins().append(Pin("p2", 3, 1, PinType::Output));

    library->getElements().append(el1);
    library->getElements().append(el2);
    library->getElements().append(el3);
    library->getElements().append(el4);

    libraries.append(library);
}

void LeeRoutingTest::routingTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0),
          Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Pin, 1, "p4"), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p2"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p3"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p5"), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    g->getCells().append(cells);

    g->getWiresData().append(WireData(1, QPoint(1, 1), QPoint(3, 7), WirePosition::Internal));
    g->getWiresData().append(WireData(0, QPoint(4, 1), QPoint(5, 4), WirePosition::Internal));
    g->getWiresData().append(WireData(2, QPoint(1, 1), QPoint(6, 7), WirePosition::Internal));

    Scheme* s = new Scheme();
    s->getElements().append(SchemeElement("lib", "el4", 0));
    s->getElements().append(SchemeElement("lib", "el3", 1));

    s->getWires().append(Wire(0, "p2", 1, "p4", WireType::Outer, 0));
    s->getWires().append(Wire(0, "p1", 1, "p3", WireType::Outer, 1));
    s->getWires().append(Wire(0, "p1", 1, "p5", WireType::Outer, 2));

    LeeRouting lee(g, s, 1);

    lee.execute();

    QVERIFY(g->getCells()[1][0].getType() == CellType::DR);
    QVERIFY(g->getCells()[2][0].getType() == CellType::UD);
    QVERIFY(g->getCells()[3][0].getType() == CellType::UDR);
    QVERIFY(g->getCells()[4][0].getType() == CellType::UD);
    QVERIFY(g->getCells()[5][0].getType() == CellType::UD);
    QVERIFY(g->getCells()[6][0].getType() == CellType::UD);
    QVERIFY(g->getCells()[7][0].getType() == CellType::UR);
    QVERIFY(g->getCells()[7][1].getType() == CellType::LR);
    QVERIFY(g->getCells()[7][2].getType() == CellType::LR);

    QVERIFY(g->getCells()[1][5].getType() == CellType::DL);
    QVERIFY(g->getCells()[2][5].getType() == CellType::UD);
    QVERIFY(g->getCells()[3][5].getType() == CellType::UDLR);

    QVERIFY(g->getCells()[3][1].getType() == CellType::LR);
    QVERIFY(g->getCells()[3][2].getType() == CellType::LR);
    QVERIFY(g->getCells()[3][3].getType() == CellType::LR);
    QVERIFY(g->getCells()[3][4].getType() == CellType::LR);

    QVERIFY(g->getCells()[3][6].getType() == CellType::LR);
    QVERIFY(g->getCells()[3][7].getType() == CellType::DL);
    QVERIFY(g->getCells()[4][7].getType() == CellType::UD);
    QVERIFY(g->getCells()[5][7].getType() == CellType::UD);
    QVERIFY(g->getCells()[6][7].getType() == CellType::UD);
    QVERIFY(g->getCells()[7][7].getType() == CellType::UL);

    delete g;
    delete s;
}

void LeeRoutingTest::branchingTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::UDLR),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty),
          Cell(CellType::UDLR), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::UDLR), Cell(CellType::UDLR), Cell(CellType::UDLR), Cell(CellType::UDLR), Cell(CellType::Empty),
          Cell(CellType::UDLR), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Pin, 1, "p1"), Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p2"), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::UDLR), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::UDLR), Cell(CellType::UDLR), Cell(CellType::UDLR), Cell(CellType::UDLR), Cell(CellType::UDLR),
          Cell(CellType::Empty), Cell(CellType::UDLR), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Pin, 2, "p1"), Cell(CellType::Element, 2), Cell(CellType::Element, 2), Cell(CellType::Pin, 2, "p2"), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Pin, 3, "p1"), Cell(CellType::Element, 3), Cell(CellType::Element, 3), Cell(CellType::Pin, 3, "p2") },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::UDLR),
          Cell(CellType::Empty), Cell(CellType::UDLR), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::UDLR),
          Cell(CellType::Empty), Cell(CellType::Pin, 4, "p1"), Cell(CellType::Element, 4), Cell(CellType::Element, 4), Cell(CellType::Pin, 4, "p2") },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::UDLR), Cell(CellType::UDLR), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    g->getCells().append(cells);

    g->getWiresData().append(WireData(0, QPoint(3, 1), QPoint(3, 3), WirePosition::Internal));
    g->getWiresData().append(WireData(0, QPoint(3, 1), QPoint(3, 5), WirePosition::Internal));
    g->getWiresData().append(WireData(0, QPoint(3, 1), QPoint(6, 5), WirePosition::Internal));
    g->getWiresData().append(WireData(0, QPoint(3, 1), QPoint(6, 7), WirePosition::Internal));

    Scheme* s = new Scheme();
    s->getElements().append(SchemeElement("lib", "el4", 0));
    s->getElements().append(SchemeElement("lib", "el4", 1));
    s->getElements().append(SchemeElement("lib", "el4", 2));
    s->getElements().append(SchemeElement("lib", "el4", 3));
    s->getElements().append(SchemeElement("lib", "el4", 4));

    s->getWires().append(Wire(0, "p2", 1, "p2", WireType::Outer, 0));
    s->getWires().append(Wire(0, "p2", 2, "p2", WireType::Outer, 1));
    s->getWires().append(Wire(0, "p2", 3, "p1", WireType::Outer, 2));
    s->getWires().append(Wire(0, "p2", 4, "p1", WireType::Outer, 3));

    LeeRouting lee(g, s, 1);

    lee.execute();

    QVERIFY(g->getCells()[1][4].getType() == CellType::DL);
    QVERIFY(g->getCells()[2][4].getType() == CellType::UD);
    QVERIFY(g->getCells()[3][4].getType() == CellType::LRU);
    QVERIFY(g->getCells()[3][5].getType() == CellType::DL);
    QVERIFY(g->getCells()[4][5].getType() == CellType::UD);
    QVERIFY(g->getCells()[5][5].getType() == CellType::UDLRI);
    QVERIFY(g->getCells()[5][4].getType() == CellType::LR);
    QVERIFY(g->getCells()[6][5].getType() == CellType::UD);
    QVERIFY(g->getCells()[7][5].getType() == CellType::UR);

    delete g;
    delete s;
}

void LeeRoutingTest::wireSortingTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0),
          Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Pin, 1, "p4"), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p2"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p3"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p5"), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    g->getCells().append(cells);

    g->getWiresData().append(WireData(0, QPoint(1, 1), QPoint(3, 6), WirePosition::Internal));
    g->getWiresData().append(WireData(1, QPoint(4, 1), QPoint(3, 5), WirePosition::Internal));
    g->getWiresData().append(WireData(2, QPoint(4, 1), QPoint(6, 7), WirePosition::Internal));
    g->getWiresData().append(WireData(3, QPoint(4, 1), QPoint(3, 7), WirePosition::Internal));

    Scheme* s = new Scheme();
    s->getElements().append(SchemeElement("lib", "el4", 0));
    s->getElements().append(SchemeElement("lib", "el3", 1));

    s->getWires().append(Wire(0, "p1", 1, "p2", WireType::Outer, 0));
    s->getWires().append(Wire(0, "p2", 1, "p1", WireType::Outer, 1));

    s->getWires().append(Wire(0, "p2", 1, "p5", WireType::Inner, 2));
    s->getWires().append(Wire(0, "p2", 1, "p3", WireType::Inner, 3));

    LeeRouting lee(g, s, 1);

    lee.execute();

    QVERIFY(g->getRoutedWires()[0] == 3);
    QVERIFY(g->getRoutedWires()[1] == 2);
    QVERIFY(g->getRoutedWires()[2] == 1);
    QVERIFY(g->getRoutedWires()[3] == 0);

    delete g;
    delete s;
}

void LeeRoutingTest::excludedWiresTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0),
          Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Pin, 1, "p4"), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p2"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p3"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p5"), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    g->getCells().append(cells);

    g->getWiresData().append(WireData(0, QPoint(1, 1), QPoint(3, 6), WirePosition::External));
    g->getWiresData().append(WireData(1, QPoint(4, 1), QPoint(3, 5), WirePosition::Internal));
    g->getWiresData().append(WireData(2, QPoint(4, 1), QPoint(6, 7), WirePosition::Internal));

    g->getRoutedWires().append(1);
    g->getRoutedWires().append(2);

    Scheme* s = new Scheme();
    s->getElements().append(SchemeElement("lib", "el4", 0));
    s->getElements().append(SchemeElement("lib", "el3", 1));

    s->getWires().append(Wire(0, "p1", 1, "p2", WireType::Outer, 0));
    s->getWires().append(Wire(0, "p2", 1, "p1", WireType::Outer, 1));

    s->getWires().append(Wire(0, "p2", 1, "p5", WireType::Inner, 2));

    LeeRouting lee(g, s, 1);

    lee.execute();

    QVERIFY(g->getRoutedWires().size() == 2);

    delete g;
    delete s;
}

void LeeRoutingTest::extendHorizontallyTest()
{
   Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p2"),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Pin, 3, "p4"), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::UD), Cell(CellType::UD), Cell(CellType::UD), Cell(CellType::Pin, 3, "p1"),
          Cell(CellType::Element, 3), Cell(CellType::Element, 3), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 2, "p4"), Cell(CellType::Empty),
          Cell(CellType::UD), Cell(CellType::UD), Cell(CellType::UD), Cell(CellType::Pin, 3, "p2"),
          Cell(CellType::Element, 3), Cell(CellType::Element, 3), Cell(CellType::Empty) },

        { Cell(CellType::Pin, 2, "p1"), Cell(CellType::Element, 2), Cell(CellType::Element, 2), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 3, "p3"),
          Cell(CellType::Element, 3), Cell(CellType::Element, 3), Cell(CellType::Pin, 3, "p5") },

        { Cell(CellType::Pin, 2, "p2"), Cell(CellType::Element, 2), Cell(CellType::Element, 2), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Pin, 2, "p3"), Cell(CellType::Element, 2), Cell(CellType::Element, 2), Cell(CellType::Pin, 2, "p5"),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p2") }
    };

    g->getCells().append(cells);

    g->getWiresData().append(WireData(0, QPoint(7, 5), QPoint(3, 0), WirePosition::Internal));

    Scheme* s = new Scheme();
    s->getElements().append(SchemeElement("lib", "el4", 0));
    s->getElements().append(SchemeElement("lib", "el4", 1));
    s->getElements().append(SchemeElement("lib", "el3", 2));
    s->getElements().append(SchemeElement("lib", "el3", 3));

    s->getWires().append(Wire(1, "p1", 0, "p2", WireType::Outer, 0));

    LeeRouting lee(g, s, 1);

    lee.execute();

    QVERIFY(g->getHeight() == 6);
    QVERIFY(g->getWidth() == 12);

    QVERIFY(g->getWiresData()[0].getSrcCoord() == QPoint(8, 5));
    QVERIFY(g->getWiresData()[0].getDestCoord() == QPoint(3, 0));

    QVERIFY(g->getRoutedWires()[0] == 0);

    QVERIFY(g->getCells()[0][4].getType() == CellType::DL);
    QVERIFY(g->getCells()[1][4].getType() == CellType::UD);
    QVERIFY(g->getCells()[2][4].getType() == CellType::UD);
    QVERIFY(g->getCells()[3][4].getType() == CellType::UR);
    QVERIFY(g->getCells()[3][5].getType() == CellType::LR);
    QVERIFY(g->getCells()[3][6].getType() == CellType::LR);
    QVERIFY(g->getCells()[3][7].getType() == CellType::DL);
    QVERIFY(g->getCells()[4][7].getType() == CellType::UD);
    QVERIFY(g->getCells()[5][7].getType() == CellType::UR);

    delete g;
    delete s;
}

void LeeRoutingTest::extendVerticallyTest()
{
    Grid* g = new Grid();

     QList<QList<Cell>> cells =
     {
         { Cell(CellType::LR), Cell(CellType::Pin, 4, "p1"), Cell(CellType::Element, 4), Cell(CellType::Element, 4),
           Cell(CellType::Pin, 4, "p2"), Cell(CellType::Pin, 2, "p1"), Cell(CellType::Element, 2), Cell(CellType::Element, 2),
           Cell(CellType::Pin, 2, "p1"), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"), Cell(CellType::Element, 1),
           Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p2")},

         { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty)},

         { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::LR), Cell(CellType::LR),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty)},

         { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty)},

         { Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p2"),
           Cell(CellType::Empty), Cell(CellType::Pin, 3, "p1"), Cell(CellType::Element, 3), Cell(CellType::Element, 3),
           Cell(CellType::Pin, 3, "p2"), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty)}
     };

     g->getCells().append(cells);

     g->getWiresData().append(WireData(0, QPoint(3, 4), QPoint(10, 0), WirePosition::Internal));

     Scheme* s = new Scheme();
     s->getElements().append(SchemeElement("lib", "el4", 0));
     s->getElements().append(SchemeElement("lib", "el4", 1));
     s->getElements().append(SchemeElement("lib", "el4", 2));
     s->getElements().append(SchemeElement("lib", "el4", 3));
     s->getElements().append(SchemeElement("lib", "el4", 4));

     s->getWires().append(Wire(0, "p2", 1, "p1", WireType::Outer, 0));

     LeeRouting lee(g, s, 1);

     lee.execute();

     QVERIFY(g->getHeight() == 6);
     QVERIFY(g->getWidth() == 14);

     QVERIFY(g->getWiresData()[0].getSrcCoord() == QPoint(3, 5));
     QVERIFY(g->getWiresData()[0].getDestCoord() == QPoint(10, 0));

     QVERIFY(g->getRoutedWires()[0] == 0);

     QVERIFY(g->getCells()[5][4].getType() == CellType::UL);
     QVERIFY(g->getCells()[4][4].getType() == CellType::UD);
     QVERIFY(g->getCells()[3][4].getType() == CellType::UD);
     QVERIFY(g->getCells()[2][4].getType() == CellType::DR);
     QVERIFY(g->getCells()[2][5].getType() == CellType::LR);
     QVERIFY(g->getCells()[2][6].getType() == CellType::LR);
     QVERIFY(g->getCells()[2][7].getType() == CellType::LR);
     QVERIFY(g->getCells()[2][8].getType() == CellType::LR);
     QVERIFY(g->getCells()[2][9].getType() == CellType::UL);
     QVERIFY(g->getCells()[1][9].getType() == CellType::UD);
     QVERIFY(g->getCells()[0][9].getType() == CellType::DR);

     delete g;
     delete s;
}

void LeeRoutingTest::extensionIsUnavailableTest()
{
    Grid* g = new Grid();

     QList<QList<Cell>> cells =
     {
         { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::UD), Cell(CellType::Pin, 2, "p1"), Cell(CellType::Element, 2),
           Cell(CellType::Element, 2), Cell(CellType::Pin, 2, "p2"), Cell(CellType::Empty), Cell(CellType::Empty)},

         { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::UD), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty)},

         { Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p2"),
           Cell(CellType::Empty), Cell(CellType::UDL), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Pin, 1, "p1"), Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p2")},

         { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::UD), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty)},

         { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
           Cell(CellType::Empty), Cell(CellType::UD), Cell(CellType::Pin, 3, "p1"), Cell(CellType::Element, 3),
           Cell(CellType::Element, 3), Cell(CellType::Pin, 3, "p2"), Cell(CellType::Empty), Cell(CellType::Empty)},
     };

     g->getCells().append(cells);

     g->getWiresData().append(WireData(0, QPoint(8, 2), QPoint(3, 2), WirePosition::Internal));

     Scheme* s = new Scheme();
     s->getElements().append(SchemeElement("lib", "el4", 0));
     s->getElements().append(SchemeElement("lib", "el4", 1));
     s->getElements().append(SchemeElement("lib", "el4", 2));
     s->getElements().append(SchemeElement("lib", "el4", 3));

     s->getWires().append(Wire(1, "p1", 0, "p2", WireType::Outer, 0));

     LeeRouting lee(g, s, 1);

     lee.execute();

     QVERIFY(g->getRoutedWires().size() == 0);

     QVERIFY(g->getHeight() == 5);
     QVERIFY(g->getWidth() == 12);

     delete g;
     delete s;
}

void LeeRoutingTest::maxExtensionAttemptsReachedTest()
{
    Grid* g = new Grid();

     QList<QList<Cell>> cells =
     {
         { Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p2"),
           Cell(CellType::Empty), Cell(CellType::UDLRI), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"),
           Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p2")}
     };

     for(QList<Cell> list: cells)
         g->getCells().append(list);

     g->getWiresData().append(WireData(0, QPoint(7, 0), QPoint(3, 0), WirePosition::Internal));

     Scheme* s = new Scheme();
     s->getElements().append(SchemeElement("lib", "el4", 0));
     s->getElements().append(SchemeElement("lib", "el4", 1));

     s->getWires().append(Wire(1, "p1", 0, "p2", WireType::Outer, 0));

     LeeRouting lee(g, s, 2);

     lee.execute();

     QVERIFY(g->getRoutedWires().size() == 0);

     QVERIFY(g->getHeight() == 1);
     QVERIFY(g->getWidth() == 11);

     QVERIFY(g->getWiresData()[0].getSrcCoord() == QPoint(7, 0));
     QVERIFY(g->getWiresData()[0].getDestCoord() == QPoint(3, 0));

     delete g;
     delete s;
}

QTEST_MAIN(LeeRoutingTest)

#include "tst_leeroutingtest.moc"
