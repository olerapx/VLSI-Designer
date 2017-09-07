#pragma once

#include "datamodels/library/library.h"
#include "datamodels/scheme/schemeelement.h"

class LibraryUtils
{
public:
    static LibraryElement getCorrespondingElement(SchemeElement element, QList<Library*>& libraries);
    static Pin findPinById(LibraryElement element, QString id);
};
