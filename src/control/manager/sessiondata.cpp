#include "sessiondata.h"

SessionData::SessionData() :
    scheme(nullptr),
    architecture(nullptr),
    empty(true)
{

}

SessionData::SessionData(Scheme* s, Architecture* a, QList<Library*> libraries) :
    scheme(s),
    architecture(a),
    libraries(libraries),
    empty(false)
{

}

SessionData::~SessionData()
{
    delete scheme;
    delete architecture;

    for(Library* l: libraries)
        delete l;

    libraries.clear();
}
