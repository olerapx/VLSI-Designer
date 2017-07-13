#include "libraryutils.h"

LibraryElement LibraryUtils::getCorrespondingElement(SchemeElement element, QList<Library *> &libraries)
{
    for(Library* l: libraries)
    {
        if(l->getId() == element.getLibraryId())
        {
            for(LibraryElement& el: l->getElements())
            {
                if (el.getId() == element.getElementId())
                    return el;
            }
        }
    }

    throw Exception(QObject::tr("Corresponding library element cannot be found, passed element with id: %1.")
                    .arg(element.getElementId()));
}
