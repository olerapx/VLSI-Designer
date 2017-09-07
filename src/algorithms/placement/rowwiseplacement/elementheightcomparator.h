#pragma once

#include "util/misc/libraryutils.h"

class ElementHeightComparator
{
public:
    ElementHeightComparator(QList<Library*>& libraries);

    bool operator ()(SchemeElement& el1, SchemeElement& el2);

private:
    QList<Library*> libraries;
};
