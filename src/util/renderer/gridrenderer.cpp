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
    for(QFileInfo& info: QDir(":/resources/images/grid").entryInfoList())
    {
        QImage image = readImageFromFile(info.absoluteFilePath());
        //TODO: try-catch-log

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

QImage GridRenderer::render(Grid *g)
{
    if(g->getCells().size() == 0)
        throw IllegalArgumentException("The grid is empty.");

    int size = g->getCells()[0].size();
    for(QList<Cell> list: g->getCells())
        if(list.size() != size)
            throw IllegalArgumentException("The grid is not rectangular.");

    this->grid = g;

    QImage res(imageSize * grid->getCells().at(0).size(), imageSize * grid->getCells().size(), QImage::Format_ARGB32);
    res.fill(Qt::white);

    for(QList<Cell> list: grid->getCells())
    {
        for(Cell cell: list)
        {
            renderCell(res, cell);
            currentX ++;
        }

        currentX = 0;
        currentY ++;
    }

    return res;
}

void GridRenderer::renderCell(QImage &image, Cell cell)
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
            renderPin(image);
            break;
        }
        case CellType::Element:
        {
            renderElement(image);
            break;
        }
        default:
            renderWire(image, cell.getType());
    }
}

void GridRenderer::renderEmpty(QImage& image)
{
    QImage tile = getImageFromCache("empty");
    renderTileOnCurrentPosition(image, tile);
}

void GridRenderer::renderPin(QImage& image)
{
    QImage tile = getImageFromCache("pin");

    if(currentY < grid->getCells().size()-1 && grid->getCells()[currentY+1][currentX].getType() == CellType::Element)
        tile = rotateImage(tile, 90);
    else if(currentX > 0 && grid->getCells()[currentY][currentX-1].getType() == CellType::Element)
        tile = rotateImage(tile, 180);
    else if(currentY > 0 && grid->getCells()[currentY-1][currentX].getType() == CellType::Element)
        tile = rotateImage(tile, 270);

    renderTileOnCurrentPosition(image, tile);
}

void GridRenderer::renderElement(QImage& image)
{
    std::bitset<4> nearbyElements(0); // top, bottom, left, right
    if(currentY > 0 && grid->getCells()[currentY-1][currentX].getType() == CellType::Element)
        nearbyElements[3] = 1;
    if(currentY < grid->getCells().size()-1 && grid->getCells()[currentY+1][currentX].getType() == CellType::Element)
        nearbyElements[2] = 1;
    if(currentX > 0 && grid->getCells()[currentY][currentX-1].getType() == CellType::Element)
        nearbyElements[1] = 1;
    if(currentX < grid->getCells()[0].size()-1 && grid->getCells()[currentY][currentX+1].getType() == CellType::Element)
        nearbyElements[0] = 1;

    ulong res = nearbyElements.to_ulong();

    QImage tile;

    switch(res)
    {
        case 0:
        {
            tile = getImageFromCache("element_border_4");
            break;
        }
        case 1:
        {
            tile = getImageFromCache("element_border_3");
            tile = rotateImage(tile, 180);
            break;
        }
        case 2:
        {
            tile = getImageFromCache("element_border_3");
            break;
        }
        case 3:
        {
            tile = getImageFromCache("element_border_2");
            tile = rotateImage(tile, 90);
            break;
        }
        case 4:
        {
            tile = getImageFromCache("element_border_3");
            tile = rotateImage(tile, -90);
            break;
        }
        case 5:
        {
            tile = getImageFromCache("element_border_2_corner");
            tile = rotateImage(tile, 90);
            break;
        }
        case 6:
        {
            tile = getImageFromCache("element_border_2_corner");
            tile = rotateImage(tile, 180);
            break;
        }
        case 7:
        {
            tile = getImageFromCache("element_border_1");
            tile = rotateImage(tile, 90);
            break;
        }
        case 8:
        {
            tile = getImageFromCache("element_border_3");
            tile = rotateImage(tile, 90);
            break;
        }
        case 9:
        {
            tile = getImageFromCache("element_border_2_corner");
            break;
        }
        case 10:
        {
            tile = getImageFromCache("element_border_2_corner");
            tile = rotateImage(tile, -90);
            break;
        }
        case 11:
        {
            tile = getImageFromCache("element_border_1");
            tile = rotateImage(tile, -90);
            break;
        }
        case 12:
        {
            tile = getImageFromCache("element_border_2");
            break;
        }
        case 13:
        {
            tile = getImageFromCache("element_border_1");
            break;
        }
        case 14:
        {
            tile = getImageFromCache("element_border_1");
            tile = rotateImage(tile, 180);
            break;
        }
        case 15:
        {
            tile = getImageFromCache("element_body");
            break;
        }
    }

    renderTileOnCurrentPosition(image, tile);
}

void GridRenderer::renderWire(QImage& image, CellType type)
{
    QImage tile;

    switch(type)
    {
        case CellType::UD:
        {
            tile = getImageFromCache("ud");
            break;
        }
        case CellType::LR:
        {
            tile = getImageFromCache("ud");
            tile = rotateImage(tile, 90);
            break;
        }
        case CellType::UL:
        {
            tile = getImageFromCache("ur");
            tile = rotateImage(tile, -90);
            break;
        }
        case CellType::UR:
        {
            tile = getImageFromCache("ur");
            break;
        }
        case CellType::DL:
        {
            tile = getImageFromCache("ur");
            tile = rotateImage(tile, 180);
            break;
        }
        case CellType::DR:
        {
            tile = getImageFromCache("ur");
            tile = rotateImage(tile, 90);
            break;
        }
        case CellType::UDL:
        {
            tile = getImageFromCache("udl");
            break;
        }
        case CellType::UDR:
        {
            tile = getImageFromCache("udl");
            tile = rotateImage(tile, 180);
            break;
        }
        case CellType::LRU:
        {
            tile = getImageFromCache("udl");
            tile = rotateImage(tile, 90);
            break;
        }
        case CellType::LRD:
        {
            tile = getImageFromCache("udl");
            tile = rotateImage(tile, -90);
            break;
        }
        case CellType::UDLR:
        {
            tile = getImageFromCache("udlr");
            break;
        }
        default:
            return;
    }

    renderTileOnCurrentPosition(image, tile);
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
