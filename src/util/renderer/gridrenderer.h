#ifndef GRIDRENDERER_H
#define GRIDRENDERER_H

#include <QImage>
#include <QMap>
#include <QDir>
#include <QPainter>
#include <bitset>

#include "datamodels/grid/grid.h"
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

    QImage render(Grid* g);

signals:
    void sendLog(QString log);

private:
    int imageSize;
    QMap<QString, QImage> imageCache;

    Grid* grid;

    int currentX;
    int currentY;

    void fillCache();
    QImage readImageFromFile(const QString filePath);

    void renderCell(QImage& image, Cell cell);
    void renderEmpty(QImage& image);
    void renderPin(QImage& image);
    void renderElement(QImage& image);
    void renderWire(QImage& image, CellType type);

    QImage getImageFromCache(QString key);
    void renderTileOnCurrentPosition(QImage& image, QImage tile);
    QImage rotateImage(QImage image, int degrees);
};

#endif // GRIDRENDERER_H
