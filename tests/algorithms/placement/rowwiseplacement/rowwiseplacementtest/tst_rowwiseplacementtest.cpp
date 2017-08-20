#include <QtTest>
#include <QCoreApplication>
#include <algorithms/placement/rowwiseplacement/rowwiseplacement.h>

class RowWisePlacementTest : public QObject
{
    Q_OBJECT

public:
    RowWisePlacementTest();
    ~RowWisePlacementTest() {}

private slots:
    void gridSquareTest();
    void elementsNumberTest();
    void rowsTest();
    void intervalsTest();

private:
    QList<Library*> libraries;
};

RowWisePlacementTest::RowWisePlacementTest()
{
    Library* library = new Library("lib", 1.0);

    library->getElements().append(LibraryElement("el1", 3, 3));
    library->getElements().append(LibraryElement("el2", 3, 4));
    library->getElements().append(LibraryElement("el3", 3, 6));

    libraries.append(library);
}

void RowWisePlacementTest::gridSquareTest()
{
    Scheme* s = new Scheme();

    for(int i=0; i<9; i++)
        s->getElements().append(SchemeElement("lib", "el1", i));

    RowWisePlacement placement(s, libraries, 2);

    PlacementResult* result = placement.execute();
    Grid* g = result->getGrid();

    QVERIFY(g->getCells().size() == 18);
    QVERIFY(g->getCells()[0].size() == 18);

    delete g;
    delete result;

    s->getElements().clear();
    for(int i=0; i<5; i++)
        s->getElements().append(SchemeElement("lib", "el1", i));

    placement.setParameters(s, libraries, 2);

    result = placement.execute();
    g = result->getGrid();

    QVERIFY(g->getCells().size() == (3 + 3 + 3) * 2);
    QVERIFY(g->getCells()[0].size() == 7 * 2);

    delete g;
    delete result;

    delete s;
}

void RowWisePlacementTest::elementsNumberTest()
{
    Scheme* s = new Scheme();

    s->getElements().append({ SchemeElement("lib", "el1", 0), SchemeElement("lib", "el2", 1),
                              SchemeElement("lib", "el3", 2), SchemeElement("lib", "el1", 3)});

    RowWisePlacement placement(s, libraries, 1.1);
    PlacementResult* res = placement.execute();

    QVERIFY(res->getElementCoordinates().size() == s->getElements().size());

    delete res->getGrid();
    delete res;

    delete s;
}

void RowWisePlacementTest::rowsTest()
{
    Scheme* s = new Scheme();

    s->getElements().append({ SchemeElement("lib", "el1", 0), SchemeElement("lib", "el2", 1),
                             SchemeElement("lib", "el3", 2), SchemeElement("lib", "el1", 3),
                             SchemeElement("lib", "el2", 4)});

    RowWisePlacement placement(s, libraries, 1.1);
    PlacementResult* res = placement.execute();

    QVERIFY(res->getRowWiseCoordinates().size() == 3);

    QVERIFY(res->getRowWiseCoordinates()[0].size() == 2);
    QVERIFY(res->getRowWiseCoordinates()[1].size() == 1);
    QVERIFY(res->getRowWiseCoordinates()[2].size() == 2);

    delete res;
    delete s;
}

void RowWisePlacementTest::intervalsTest()
{
    Scheme* s = new Scheme();

    s->getElements().append({ SchemeElement("lib", "el1", 0), SchemeElement("lib", "el2", 1),
                             SchemeElement("lib", "el3", 2), SchemeElement("lib", "el1", 3),
                             SchemeElement("lib", "el2", 4)});

    RowWisePlacement placement(s, libraries, 2);
    PlacementResult* res = placement.execute();

    QVERIFY(res->getRowWiseCoordinates()[0][0].getTopLeftCoord().y() == 2);
    QVERIFY(res->getRowWiseCoordinates()[1][0].getTopLeftCoord().y() == 2 + 3 + 3);
    QVERIFY(res->getRowWiseCoordinates()[2][0].getTopLeftCoord().y() == 2 +3 + 3 + 3 + 3);

    QList<ElementCoordinate> firstRow = res->getRowWiseCoordinates()[0];

    QVERIFY(firstRow[0].getTopLeftCoord().x() == 2);
    QVERIFY(firstRow[1].getTopLeftCoord().x() == 2 + libraries[0]->getElements()[0].getWidth() + 3);

    delete res;
    delete s;
}

QTEST_MAIN(RowWisePlacementTest)

#include "tst_rowwiseplacementtest.moc"
