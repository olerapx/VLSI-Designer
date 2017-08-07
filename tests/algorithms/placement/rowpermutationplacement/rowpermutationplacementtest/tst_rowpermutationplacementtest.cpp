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

    LibraryElement el1("el1", 3, 4);
    el1.getPins().append(Pin("p1", 0, 1, PinType::Input));
    el1.getPins().append(Pin("p2", 0, 2, PinType::Input));
    el1.getPins().append(Pin("p3", 2, 2, PinType::Output));

    library->getElements().append(el1);
    library->getElements().append(LibraryElement("el2", 3, 4));
    library->getElements().append(LibraryElement("el3", 3, 6));

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
        Wire(1, "p3", 0, "p2", WireType::Inner, 3)
    };

    PlacementResult res(g, coords, wires, libraries);

    RowPermutationPlacement placement(&res);
    placement.execute();

    QVERIFY(placement.getFitnessValue() == (1 + 2 + 5 + 30));

    delete g;
}

void RowPermutationPlacementTest::elementsPermutationTest()
{

}

QTEST_MAIN(RowPermutationPlacementTest)

#include "tst_rowpermutationplacementtest.moc"
