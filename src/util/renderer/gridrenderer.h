#ifndef GRIDRENDERER_H
#define GRIDRENDERER_H

#include <QImage>
#include <QMap>
#include <QDir>
#include "datamodels/grid/grid.h"
#include "exception/ioexception.h"

/**
 * @brief The GridRenderer class
 * Constructs an image of a grid using the image resources.
 * Each cell in the grid is associated with one of the images, so the resulting image is composed of them.
 */
class GridRenderer
{
public:
    GridRenderer();

    QImage render(Grid g);

private:
    int imageSize;
    QMap<QString, QImage> imageCache;

    int currentX;
    int currentY;

    void fillCache();
    QImage readImageFromFile(const QString filePath);

    void renderCell(QImage& image, Cell cell, Grid& g);
    void renderEmpty(QImage& image, Grid& g);
    void renderPin(QImage& image, Grid& g);
    void renderElement(QImage& image, Grid& g);
    void renderWire(QImage& image, Grid& g);

    QImage getImageFromCache(QString key);
    QImage rotateImage(QImage image, int degrees);
};

#endif // GRIDRENDERER_H
