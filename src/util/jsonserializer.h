#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "src/datamodels/architecture/architecture.h"
#include "src/datamodels/grid/grid.h"
#include "src/datamodels/library/library.h"
#include "src/datamodels/scheme/scheme.h"

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
