#include <QtTest>
#include <QCoreApplication>

#include <algorithms/placement/rowpermutationplacement/rowpermutationplacement.h>

class RowPermutationPlacementTest : public QObject
{
    Q_OBJECT

public:
    RowPermutationPlacementTest();
    ~RowPermutationPlacementTest() {}

private slots:
    void fitnessFunctionTest();
    void elementsPermutationTest();

private:
    QList<Library*> libraries;
};

RowPermutationPlacementTest::RowPermutationPlacementTest()
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

    library->getElements().append(el1);
    library->getElements().append(el2);

    libraries.append(library);
}

void RowPermutationPlacementTest::fitnessFunctionTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 0 , "p1"),
          Cell(CellType::Element, 0), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Pin, 0 , "p2"),
          Cell(CellType::Element, 0), Cell(CellType::Pin, 0 , "p3") },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Pin, 1, "p1"), Cell(CellType::Element, 1), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty) },

        { Cell(CellType::Pin, 1, "p2"), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p3"),
          Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    for(QList<Cell> list: cells)
        g->getCells().append(list);

    QList<ElementCoordinate> coords =
    {
        ElementCoordinate(SchemeElement("lib", "el1", 0), QPoint(2, 0)),
        ElementCoordinate(SchemeElement("lib", "el1", 1), QPoint(0, 4))
    };

    QList<Wire> wires =
    {
        Wire(0, "p1", 10, "dummy", WireType::Outer, 0),
        Wire(0, "p2", 11, "dummy1", WireType::Outer, 1),
        Wire(0, "p3", 1, "p3", WireType::Outer, 2),
        Wire(1, "p3", 0, "p2", WireType::Inner, 3),
        Wire(0, "p3", 0, "p2", WireType::Outer, 4)
    };

    PlacementResult res(g, coords, wires, libraries);

    RowPermutationPlacement placement(&res);

    QVERIFY(placement.getFitnessValue() == (1 + 2 + 5 + 30 + 1));

    delete g;
}

void RowPermutationPlacementTest::elementsPermutationTest()
{
    Grid* g = new Grid();

    QList<QList<Cell>> cells =
    {
        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }  ,

        { Cell(CellType::Pin, 0, "p1"), Cell(CellType::Element, 0), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }  ,

        { Cell(CellType::Pin, 0, "p2"), Cell(CellType::Element, 0), Cell(CellType::Pin, 0, "p3"),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }  ,

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }  ,

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }  ,

        { Cell(CellType::Pin, 1, "p1"), Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Empty),
          Cell(CellType::Pin, 2, "p1"), Cell(CellType::Element, 2), Cell(CellType::Empty),
          Cell(CellType::Pin, 3, "p1"), Cell(CellType::Element, 3), Cell(CellType::Empty) }  ,

        { Cell(CellType::Pin, 1, "p2"), Cell(CellType::Element, 1), Cell(CellType::Element, 1), Cell(CellType::Pin, 1, "p3"),
          Cell(CellType::Pin, 2, "p2"), Cell(CellType::Element, 2), Cell(CellType::Pin, 2, "p3"),
          Cell(CellType::Pin, 3, "p2"), Cell(CellType::Element, 3), Cell(CellType::Pin, 3, "p3") }  ,

        { Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty),
          Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty), Cell(CellType::Empty) }
    };

    for(QList<Cell> list: cells)
        g->getCells().append(list);

    QList<ElementCoordinate> coords =
    {
        ElementCoordinate(SchemeElement("lib", "el1", 0), QPoint(0, 0)),
        ElementCoordinate(SchemeElement("lib", "el2", 1), QPoint(0, 4)),
        ElementCoordinate(SchemeElement("lib", "el1", 2), QPoint(4, 4)),
        ElementCoordinate(SchemeElement("lib", "el1", 3), QPoint(7, 4))
    };

    QList<Wire> wires =
    {
        Wire(0, "p3", 0, "p2", WireType::Outer, 0),
        Wire(3, "p3", 1, "p1", WireType::Outer, 1),
        Wire(3, "p3", 2, "p2", WireType::Outer, 2),
        Wire(10, "dummy", 0, "p1", WireType::Outer, 3)
    };

    PlacementResult* res = new PlacementResult(g, coords, wires, libraries);

    RowPermutationPlacement placement(res);

    QVERIFY(placement.getFitnessValue() == (1 + 9 + 4));

    PlacementResult* newRes = placement.execute();
    placement.setParameters(newRes);
    delete res;

    QVERIFY(placement.getFitnessValue() == 5);

    QVERIFY(newRes->getElementCoordinates()[0].getTopLeftCoord() == QPoint(0, 0));
    QVERIFY(newRes->getElementCoordinates()[1].getTopLeftCoord() == QPoint(0, 4));
    QVERIFY(newRes->getElementCoordinates()[2].getTopLeftCoord() == QPoint(3, 4));
    QVERIFY(newRes->getElementCoordinates()[3].getTopLeftCoord() == QPoint(6, 4));

    QVERIFY(newRes->getElementCoordinates()[0].getElement().getIndex() == 0);
    QVERIFY(newRes->getElementCoordinates()[1].getElement().getIndex() == 3);
    QVERIFY(newRes->getElementCoordinates()[2].getElement().getIndex() == 2);
    QVERIFY(newRes->getElementCoordinates()[3].getElement().getIndex() == 1);

    QVERIFY(newRes->getGrid()->getCells()[1][0].getIndex() == 0);
    QVERIFY(newRes->getGrid()->getCells()[5][0].getIndex() == 3);
    QVERIFY(newRes->getGrid()->getCells()[5][3].getIndex() == 2);
    QVERIFY(newRes->getGrid()->getCells()[5][6].getIndex() == 1);

    delete newRes;

    delete g;
}

QTEST_MAIN(RowPermutationPlacementTest)

#include "tst_rowpermutationplacementtest.moc"
