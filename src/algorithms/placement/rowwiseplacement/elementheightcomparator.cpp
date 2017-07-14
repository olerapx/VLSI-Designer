#include "elementheightcomparator.h"

ElementHeightComparator::ElementHeightComparator(QList<Library*> &libraries)
{
    this->libraries = libraries;
}

bool ElementHeightComparator::operator ()(SchemeElement& el1, SchemeElement& el2)
{
    LibraryElement libraryElement1 = LibraryUtils::getCorrespondingElement(el1, libraries);
    LibraryElement libraryElement2 = LibraryUtils::getCorrespondingElement(el2, libraries);

    return (libraryElement1.getHeight() < libraryElement2.getHeight());
}
