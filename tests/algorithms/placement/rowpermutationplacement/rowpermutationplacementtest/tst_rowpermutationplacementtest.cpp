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

private:
    QList<Library*> libraries;
};

RowPermutationPlacementTest::RowPermutationPlacementTest()
{
    Library* library = new Library("lib", 1.0);

    library->getElements().append(LibraryElement("el1", 3, 3));
    library->getElements().append(LibraryElement("el2", 3, 4));
    library->getElements().append(LibraryElement("el3", 3, 6));

    libraries.append(library);
}

QTEST_MAIN(RowPermutationPlacementTest)

#include "tst_rowpermutationplacementtest.moc"
