#include "gridrenderer.h"

GridRenderer::GridRenderer()
{
    imageSize = 0;
    currentX = 0;
    currentY = 0;

    fillCache();
}

void GridRenderer::fillCache()
{
    for(const QString& str: QDir(":/resources").entryList())
    {
        QImage image = readImageFromFile(str);

        QFileInfo info(str);

        if(image.width() <= 0 || image.height() <= 0 || image.isNull())
            throw Exception(QString("The resource image is empty or corrupted: %1").arg(info.fileName()));

        if(image.width() != image.height())
            throw Exception(QString("The resource image must be rectangular: %1.").arg(info.fileName()));

        if(imageSize == 0)
            imageSize = image.width();
        else if(image.width() != imageSize)
            throw Exception(QString("All resource images must have the same size: %1.").arg(info.fileName()));

        imageCache.insert(info.completeBaseName(), image);
    }
}

QImage GridRenderer::readImageFromFile(const QString filePath)
{
    QFile f(filePath);
    QFileInfo info(filePath);

    if(!f.open(QIODevice::ReadOnly))
        throw IOException(QString("The resource image file cannot be found or opened: %1").arg(info.fileName()));

    QImage image = QImage::fromData(f.readAll(), info.suffix().toLocal8Bit());
    f.close();

    if(image.isNull())
        throw Exception(QString("The resource image is not supported or corrupted.").arg(info.fileName()));

    return image;
}

QImage GridRenderer::render(Grid g)
{
    if(g.getCells().size() == 0)
        throw IllegalArgumentException("The grid is empty.");

    QImage res(imageSize * g.getCells().at(0).size(), imageSize * g.getCells().size(), QImage::Format_ARGB32);
    res.fill(Qt::white);

    for(QList<Cell> list: g.getCells())
    {
        for(Cell cell: list)
        {
            renderCell(res, cell, g);
            currentX ++;
        }

        currentX = 0;
        currentY ++;
    }

    return res;
}

void GridRenderer::renderCell(QImage &image, Cell cell, Grid &g)
{
    switch(cell.getType())
    {
        case CellType::Empty:
        {
            renderEmpty(image);
            break;
        }
        case CellType::Pin:
        {
            renderPin(image, g);
            break;
        }
        case CellType::Element:
        {
            renderElement(image, g);
            break;
        }
        default:
            renderWire(image, g, cell.getType());
    }
}

void GridRenderer::renderEmpty(QImage& image)
{
    QImage tile = getImageFromCache("empty");
    renderTileOnCurrentPosition(image, tile);
}

void GridRenderer::renderPin(QImage& image, Grid& g)
{

}

void GridRenderer::renderElement(QImage& image, Grid& g)
{

}

void GridRenderer::renderWire(QImage& image, Grid& g, CellType type)
{

}

QImage GridRenderer::getImageFromCache(QString key)
{
    if(!imageCache.contains(key))
        throw Exception(QString("Missing cache image: %1").arg(key));

    return imageCache.value(key);
}

void GridRenderer::renderTileOnCurrentPosition(QImage& image, QImage tile)
{
    QPoint position(currentX * imageSize, currentY * imageSize);

    QPainter painter(&image);
    painter.drawImage(position, tile);
    painter.end();
}

QImage GridRenderer::rotateImage(QImage image, int degrees)
{
    if(degrees % 90 != 0)
        throw IllegalArgumentException("Degrees value must be multiple of 90.");

    QTransform transform;
    transform.rotate(degrees);

    return image.transformed(transform);
}
