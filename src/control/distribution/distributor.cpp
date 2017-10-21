#include "distributor.h"

Distributor::Distributor(Client& client, FileSystem& system) :
    client(client),
    fileSystem(system)
{

}

int Distributor::getLevelsNumber() const
{
    if(client.getArchitecture() == nullptr)
        return 0;

    return client.getArchitecture()->getModel().size() + 1;
}

int Distributor::getClientsNumberOnNextLevel(int levelNumber) const
{
    if(client.getArchitecture() == nullptr)
        return 0;

    if(levelNumber == getLevelsNumber() - 1)
        return 0;

    return client.getArchitecture()->getModel()[levelNumber];
}

bool Distributor::isLastLevel(int levelNumber) const
{
    if(levelNumber == getLevelsNumber() - 1)
        return true;

    return false;
}

void Distributor::writeGrid(Grid* g, int level) const
{
    BinarySerializer serializer;

    QDir dir(fileSystem.getLevelPath(level));
    dir.mkpath(".");

    QFile f(fileSystem.getLevelPath(level) + "/grid.bin");
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(g));
    f.close();
}

void Distributor::writeGridImage(QImage image, int level) const
{
    QDir dir(fileSystem.getLevelPath(level));
    dir.mkpath(".");

    image.save(fileSystem.getLevelPath(level) + "/grid.png");
}

void Distributor::writeScheme(Scheme* s, int level) const
{
    BinarySerializer serializer;

    QDir dir(fileSystem.getLevelPath(level));
    dir.mkpath(".");

    QFile f(fileSystem.getLevelPath(level) + "/scheme.bin");
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(s));
    f.close();
}

void Distributor::writeStatistics(Statistics* s, int level) const
{
    BinarySerializer serializer;

    QDir dir(fileSystem.getStatisticsPath(level));
    dir.mkpath(".");

    QFile f(fileSystem.getStatisticsPath(level) + "/stats.bin");
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(s));
    f.close();
}

void Distributor::writeGridPart(Grid* g, int level) const
{
    int number = getGridPartsNumber(level);

    BinarySerializer serializer;

    QDir dir(fileSystem.getGridsPath(level));
    dir.mkpath(".");

    QFile f(fileSystem.getGridsPath(level) + QString("/%1.bin").arg(QString::number(number)));
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(g));
    f.close();
}

void Distributor::writeSchemePart(Scheme* s, int level) const
{
    int number = getSchemePartsNumber(level);

    BinarySerializer serializer;

    QDir dir(fileSystem.getSchemesPath(level));
    dir.mkpath(".");

    QFile f(fileSystem.getSchemesPath(level) + QString("/%1.bin").arg(QString::number(number)));
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(s));
    f.close();
}

int Distributor::getGridPartsNumber(int level) const
{
    QDir dir(fileSystem.getGridsPath(level));
    QList<QFileInfo> infos = getDirEntries(dir);

    return infos.size();
}

int Distributor::getSchemePartsNumber(int level) const
{
    QDir dir(fileSystem.getSchemesPath(level));
    QList<QFileInfo> infos = getDirEntries(dir);

    return infos.size();
}

QList<Grid*> Distributor::readGridParts(int level) const
{
    QDir dir(fileSystem.getGridsPath(level));
    QList<QFileInfo> infos = getDirEntries(dir);

    BinarySerializer serializer;    

    QList<Grid*> res;

    for(QFileInfo& info: infos)
    {
        QFile f(info.filePath());
        f.open(QIODevice::ReadOnly);

        res.append(dynamic_cast<Grid*>(serializer.deserialize(f.readAll())));

        f.close();
    }

    return res;
}

QList<Scheme*> Distributor::readSchemeParts(int level) const
{
    QDir dir(fileSystem.getSchemesPath(level));
    QList<QFileInfo> infos = getDirEntries(dir);

    BinarySerializer serializer;    

    QList<Scheme*> res;

    for(QFileInfo& info: infos)
    {
        QFile f(info.filePath());
        f.open(QIODevice::ReadOnly);

        res.append(dynamic_cast<Scheme*>(serializer.deserialize(f.readAll())));

        f.close();
    }

    return res;
}

Scheme* Distributor::readScheme(int level) const
{
    BinarySerializer serializer;

    QFile f(fileSystem.getLevelPath(level) + "/scheme.bin");
    f.open(QIODevice::ReadOnly);
    Scheme* s = dynamic_cast<Scheme*>(serializer.deserialize(f.readAll()));
    f.close();

    return s;
}

Statistics* Distributor::readStatistics(int level) const
{
    BinarySerializer serializer;

    QFile f(fileSystem.getStatisticsPath(level) + "/stats.bin");
    f.open(QIODevice::ReadOnly);
    Statistics* s = dynamic_cast<Statistics*>(serializer.deserialize(f.readAll()));
    f.close();

    return s;
}

QList<QFileInfo> Distributor::getDirEntries(QDir& dir) const
{
    QStringList filters;
    filters << "*.bin";
    QList<QFileInfo> infos = dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

    return infos;
}
