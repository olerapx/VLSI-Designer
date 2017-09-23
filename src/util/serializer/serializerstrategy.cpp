#include "serializerstrategy.h"

Serializer* SerializerStrategy::createSerializer(QFile& file)
{
    QFileInfo info(file);

    if(info.suffix().toLower() == "bin")
        return new BinarySerializer();
    else
        return new JsonSerializer();
}
