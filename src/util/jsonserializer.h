#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

#include "src/datamodels/architecture/architecture.h"
#include "src/datamodels/grid/grid.h"
#include "src/datamodels/library/library.h"
#include "src/datamodels/scheme/scheme.h"

static QMap<QString, PinType> pinTypeMap
{
    {"input", PinType::Input},
    {"output", PinType::Output}
};

static QMap<QString, DistributionType> distributionTypeMap
{
    {"default", DistributionType::Default},
    {"greedy", DistributionType::Greedy}
};

static QMap<QString, WireType> wireTypeMap
{
  {"inner", WireType::Inner},
  {"outer", WireType::Outer}
};

static QMap<QString, CellType> cellTypeMap
{
  {"empty", CellType::Empty},
  {"pin", CellType::Pin},
  {"element", CellType::Element},
  {"UD", CellType::UD},
  {"LR", CellType::LR},
  {"UL", CellType::UL},
  {"UR", CellType::UR},
  {"DL", CellType::DL},
  {"DR", CellType::DR},
  {"UDL", CellType::UDL},
  {"UDR", CellType::UDR},
  {"LRU", CellType::LRU},
  {"LRD", CellType::LRD},
  {"UDLR", CellType::UDLR},
};

/**
 * @brief The JsonSerializer class
 *
 */
class JsonSerializer
{
protected:
    Library* deserializeLibrary (QJsonObject obj);
    Element* deserializeElement (QJsonObject obj);
    Pin* deserializePin (QJsonObject obj);

    Scheme* deserializeScheme (QJsonObject obj);
    SchemeElement* deserializeSchemeElement (QJsonObject obj);
    Wire* deserializeWire (QJsonObject obj);

    Grid* deserializeGrid (QJsonObject obj);
    Cell* deserializeCell (QJsonObject obj);

    Architecture* deserializeArchitecture (QJsonObject obj);

public:
    JsonSerializer();

    Serializable* deserialize (QByteArray jsonData);
};

#endif // JSONSERIALIZER_H
