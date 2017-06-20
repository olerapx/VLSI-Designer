#ifndef GRIDRENDERER_H
#define GRIDRENDERER_H

#include <QImage>
#include <QMap>
#include <QList>
#include <QDir>
#include <QPainter>
#include <bitset>

#include "datamodels/grid/grid.h"
#include "datamodels/scheme/scheme.h"
#include "exception/ioexception.h"

/**
 * @brief The GridRenderer class
 * Constructs an image of a grid using the image resources.
 * Each cell in the grid is associated with one of the images, so the resulting image is composed of them.
 */
class GridRenderer: public QObject
{
    Q_OBJECT

public:
    GridRenderer();
    virtual ~GridRenderer() {}

    /**
     * @brief render
     * Renders the given grid.
     * @param g - grid.
     * @return image.
     */
    QImage render(Grid* g, Scheme *s);

signals:

    /**
     * @brief sendLog
     * @param log
     */
    void sendLog(QString log);

private:
    int imageSize;
    QMap<QString, QImage> imageCache;
    QList<qint64> renderedIndexes;

    Grid* grid;
    Scheme* scheme;

    int currentX;
    int currentY;

    void fillCache();
    void fillCacheFromPath(QString path);
    QImage readImageFromFile(const QString filePath);

    void renderCell(QImage& image, Cell cell);
    void renderEmpty(QImage& image);
    void renderPin(QImage& image);
    void renderElement(QImage& image, Cell cell);
    void renderAlias(QImage& image, Cell cell);
    QString lookupAlias(qint64 index);

    void renderWire(QImage& image, CellType type);    

    QImage getImageFromCache(QString key);
    void renderTileOnCurrentPosition(QImage& image, QImage tile);
    QImage rotateImage(QImage image, int degrees);
};

#endif // GRIDRENDERER_H
