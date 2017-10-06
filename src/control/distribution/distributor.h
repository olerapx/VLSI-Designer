#pragma once

#include <QDir>
#include <QFile>
#include <QHostInfo>

#include "control/pool/node/client.h"
#include "util/serializer/binaryserializer.h"
#include "util/renderer/gridrenderer.h"
#include "control/filesystem/filesystem.h"
#include "datamodels/statistics/statistics.h"

/**
 * @brief The Distributor class
 * Base class for distribution strategies.
 */
class Distributor : public QObject
{
    Q_OBJECT

public:
    Distributor(Client& client, FileSystem& system);

    /**
     * @brief start
     * Starts the design process or continues it depends on initialLevel.
     * @param scheme
     * @param initialLevel
     */
    virtual void start(Scheme* scheme, int initialLevel) = 0;
    virtual void stop() = 0;

signals:
    /**
     * @brief sendResult
     * Emits when all the necessary work is done and the grid should be sent to the sender host.
     * @param grid
     * @param level
     */
    void sendResult(Grid* grid, int level, Statistics* statistics);

    /**
     * @brief sendSchemePart
     * Emits when a scheme part should be transmitted to another node.
     * @param scheme
     * @param level
     */
    void sendSchemePart(Scheme* scheme, int level);

    /**
     * @brief sendNeedNodes
     * Emits when the nodes are needed to continue the design process.
     * @param number
     */
    void sendNeedNodes(int level, int number);

public slots:    
    virtual void onIncomingGrid(Grid* grid, int level, Statistics* statistics) = 0;
    virtual void onReceivedNodes(int level) = 0;

protected:
    int getLevelsNumber() const;
    int getClientsNumberOnNextLevel(int levelNumber) const;

    bool isLastLevel(int levelNumber) const;

    void writeGrid(Grid* g, int level) const;
    void writeGridImage(Grid* g, Scheme* s, int level) const;
    void writeScheme(Scheme* s, int level) const;
    void writeStatistics(Statistics* s, int level) const;
    void writeGridPart(Grid* g, int level) const;
    void writeSchemePart(Scheme* s, int level) const;    

    QList<QFileInfo> getDirEntries(QDir& dir) const;

    QList<Grid*> readGridParts(int level) const;
    QList<Scheme*> readSchemeParts(int level) const;
    Scheme* readScheme(int level) const;
    Statistics* readStatistics(int level) const;

    int getGridPartsNumber(int level) const;
    int getSchemePartsNumber(int level) const;

    Client& client;
    FileSystem& fileSystem;
};
