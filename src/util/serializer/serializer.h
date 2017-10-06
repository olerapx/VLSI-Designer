#pragma once

#include <QMap>

#include "datamodels/architecture/architecture.h"
#include "datamodels/grid/grid.h"
#include "datamodels/library/library.h"
#include "datamodels/scheme/scheme.h"
#include "datamodels/statistics/statistics.h"

static const QMap<QString, PinType> pinTypeMap
{
    {"input", PinType::Input},
    {"output", PinType::Output}
};

static const QMap<QString, DistributionType> distributionTypeMap
{
    {"default", DistributionType::Default},
    {"greedy", DistributionType::Greedy}
};

static const QMap<QString, WireType> wireTypeMap
{
    {"inner", WireType::Inner},
    {"outer", WireType::Outer}
};

static const QMap<QString, CellType> cellTypeMap
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
    {"UDLRI", CellType::UDLRI}
};

static const QMap<QString, WirePosition> wirePositionMap
{
    {"external", WirePosition::External},
    {"internal", WirePosition::Internal}
};

/**
 * @brief The Serializer class
 * Base class for serializers/deserializers.
 */
class Serializer
{
public:
    Serializer() {}
    virtual ~Serializer() {}

    virtual QByteArray serialize(Serializable* s) = 0;
    virtual Serializable* deserialize(QByteArray data) = 0;
};
