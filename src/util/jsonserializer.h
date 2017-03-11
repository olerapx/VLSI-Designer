#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "datamodels/architecture/architecture.h"
#include "datamodels/grid/grid.h"
#include "datamodels/library/library.h"
#include "datamodels/scheme/scheme.h"

/**
 * @brief The JsonSerializer class
 *
 */
class JsonSerializer
{
protected:
    Library deserializeLibrary (QJsonObject obj);
    Element *deserializeElement(QJsonObject obj);
    Pin *deserializePin(QJsonObject obj);

public:
    JsonSerializer();

    Serializable deserialize (QByteArray jsonData);
};

#endif // JSONSERIALIZER_H
