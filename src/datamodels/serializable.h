#pragma once

#include <QString>
#include <QObject>

#include "exception/illegalargumentexception.h"

/**
 * @brief The Serializable class
 * Base class for all models which can be serialized or deserialized.
 */
class Serializable
{
public:
    Serializable();
    virtual ~Serializable() = 0;
};
