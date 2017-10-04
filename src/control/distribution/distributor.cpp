#include "distributor.h"

Distributor::Distributor(Client& client, QString currentSessionPath) :
    client(client),
    currentSessionPath(currentSessionPath),
    initialLevel(0),
    currentLevel(0),
    scheme(nullptr)
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

QString Distributor::getLevelPath(int level) const
{
    return currentSessionPath + "/blobs/" + QString::number(level);
}

QString Distributor::getGridsPath(int level) const
{
    return getLevelPath(level) + "/grids";
}

QString Distributor::getSchemesPath(int level) const
{
    return getLevelPath(level) + "/schemes";
}

void Distributor::writeGrid(Grid* g, int level) const
{
    BinarySerializer serializer;

    QDir dir(getLevelPath(level));
    dir.mkpath(".");

    QFile f(getLevelPath(level) + "/grid.bin");
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(g));
    f.close();
}

void Distributor::writeGridImage(Grid* g, Scheme* s, int level) const
{
    GridRenderer renderer(g, s);

    QDir dir(getLevelPath(level));
    dir.mkpath(".");

    renderer.execute().save(getLevelPath(level) + "/grid.png");
}

void Distributor::writeScheme(Scheme* s, int level) const
{
    BinarySerializer serializer;

    QDir dir(getLevelPath(level));
    dir.mkpath(".");

    QFile f(getLevelPath(level) + "/scheme.bin");
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(s));
    f.close();
}

void Distributor::writeGridPart(Grid* g, int level) const
{
    int number = getGridPartsNumber(level);

    BinarySerializer serializer;

    QDir dir(getGridsPath(level));
    dir.mkpath(".");

    QFile f(getGridsPath(level) + QString("/%1.bin").arg(QString::number(number)));
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(g));
    f.close();
}

void Distributor::writeSchemePart(Scheme* s, int level) const
{
    int number = getSchemePartsNumber(level);

    BinarySerializer serializer;

    QDir dir(getSchemesPath(level));
    dir.mkpath(".");

    QFile f(getSchemesPath(level) + QString("/%1.bin").arg(QString::number(number)));
    f.open(QIODevice::WriteOnly);
    f.write(serializer.serialize(s));
    f.close();
}

int Distributor::getGridPartsNumber(int level) const
{
    QDir dir(getGridsPath(level));

    QStringList filters;
    filters << "*.bin";

    QList<QFileInfo> infos = dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

    return infos.size();
}

int Distributor::getSchemePartsNumber(int level) const
{
    QDir dir(getSchemesPath(level));

    QStringList filters;
    filters << "*.bin";

    QList<QFileInfo> infos = dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

    return infos.size();
}

QList<Grid*> Distributor::readGridParts(int level) const
{
    QDir dir(getGridsPath(level));

    QStringList filters;
    filters << "*.bin";

    BinarySerializer serializer;

    QList<QFileInfo> infos = dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

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
    QDir dir(getSchemesPath(level));

    QStringList filters;
    filters << "*.bin";

    BinarySerializer serializer;

    QList<QFileInfo> infos = dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

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

    QFile f(getLevelPath(level) + "/scheme.bin");
    f.open(QIODevice::ReadOnly);
    Scheme* s = dynamic_cast<Scheme*>(serializer.deserialize(f.readAll()));
    f.close();

    return s;
}
