#include <QtTest>
#include <QCoreApplication>
#include <algorithms/routing/leerouting/leerouting.h>
#include <algorithms/placement/rowwiseplacement/rowwiseplacement.h>
#include <util/renderer/gridrenderer.h>

class LeeRoutingTest : public QObject
{
    Q_OBJECT

public:
    LeeRoutingTest();
    ~LeeRoutingTest() {}

private slots:
    void routingTest();

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
    try
    {
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Element, 0),
          Cell(CellType::Pin, 0, "p2"), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Pin, 1, "p4"), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p1"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p2"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 1, "p3"),
          Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p5") },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    for(QList<Cell> list: cells)
        g->getCells().append(list);

    g->getWiresData().append(WireData(1, QPoint(1, 1), QPoint(3, 7), WirePosition::Internal));
    g->getWiresData().append(WireData(0, QPoint(4, 1), QPoint(5, 4), WirePosition::Internal));

    Scheme* s = new Scheme();
    s->getElements().append(SchemeElement("lib", "el4", 0));
    s->getElements().append(SchemeElement("lib", "el3", 1));

    s->getWires().append(Wire(0, "p1", 1, "p4", WireType::Outer, 0));
    s->getWires().append(Wire(0, "p2", 1, "p3", WireType::Outer, 1));

    GridRenderer renderer(g, s);
    renderer.execute().save("old.png");

    RowWisePlacement* rwp = new RowWisePlacement(s, libraries, 1.5);
    LeeRouting lee(g, s, rwp, 1);

    lee.execute();
    renderer.execute().save("new.png");

    delete g;
    delete s;

    delete rwp;
    }
    catch(Exception e)
    {
        e.what();
    }
}

QTEST_MAIN(LeeRoutingTest)

#include "tst_leeroutingtest.moc"
