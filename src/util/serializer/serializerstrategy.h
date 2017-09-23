#pragma once

#include <QFileInfo>

#include "util/serializer/jsonserializer.h"
#include "util/serializer/binaryserializer.h"

class SerializerStrategy
{
public:
    static Serializer* createSerializer(QFile& file);
};
