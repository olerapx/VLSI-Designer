#ifndef LIBRARYUTILS_H
#define LIBRARYUTILS_H

#include "datamodels/library/library.h"
#include "datamodels/scheme/schemeelement.h"

class LibraryUtils
{
public:
    static LibraryElement getCorrespondingElement(SchemeElement element, QList<Library*>& libraries);
};

#endif // LIBRARYUTILS_H
