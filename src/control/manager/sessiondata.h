#pragma once

#include "datamodels/scheme/scheme.h"
#include "datamodels/architecture/architecture.h"
#include "datamodels/library/library.h"

/**
 * @brief The SessionData class
 * Encapsulates parameters needed to start a VLSI design process.
 */
class SessionData
{
public:
    SessionData();
    SessionData(Scheme* s, Architecture* a, QList<Library*> libraries);
    ~SessionData();

    Scheme* getScheme() const { return scheme; }
    Architecture* getArchitecture() const { return architecture; }
    QList<Library*> getLibraries() { return libraries; }

    bool isEmpty() { return empty; }

private:
    Scheme* scheme;
    Architecture* architecture;
    QList<Library*> libraries;

    bool empty;
};
