#include "gridrenderer.h"

GridRenderer::GridRenderer(Grid* g, Scheme* s)
{
    setParameters(g, s);

    imageSize = 0;
    currentX = 0;
    currentY = 0;

    fillCache();
    clear();
}

void GridRenderer::setParameters(Grid* g, Scheme* s)
{
    if(!stopped)
        throw Exception(tr("Cannot change parameters while the renderer is working."));

    this->grid = g;
    this->scheme = s;
}

void GridRenderer::fillCache()
{
    fillCacheFromPath(":/resources/images/grid");
    fillCacheFromPath(":/resources/images/alias/digits");
    fillCacheFromPath(":/resources/images/alias/lower");
    fillCacheFromPath(":/resources/images/alias/upper");
}

void GridRenderer::fillCacheFromPath(QString path)
{
    for(QFileInfo& info: QDir(path).entryInfoList())
    {
        QImage image;

        try
        {
            image = readImageFromFile(info.absoluteFilePath());
        }
        catch(Exception& e)
        {
            sendLog(e.what());
            continue;
        }

        if(image.width() <= 0 || image.height() <= 0 || image.isNull())
            throw Exception(tr("The resource image is empty or corrupted: %1. Contact the developers to resolve this.").arg(info.fileName()));

        if(image.width() != image.height())
            throw Exception(tr("The resource image must be rectangular: %1. Contact the developers to resolve this.").arg(info.fileName()));

        if(imageSize == 0)
            imageSize = image.width();
        else if(image.width() != imageSize)
            throw Exception(tr("All resource images must have the same size: %1. Contact the developers to resolve this.").arg(info.fileName()));

        imageCache.insert(info.completeBaseName(), image);
    }
}

QImage GridRenderer::readImageFromFile(const QString filePath)
{
    QFile f(filePath);
    QFileInfo info(filePath);

    if(!f.open(QIODevice::ReadOnly))
        throw IOException(tr("The resource image file cannot be found or opened: %1.").arg(info.fileName()));

    QImage image = QImage::fromData(f.readAll(), info.suffix().toLocal8Bit());
    f.close();

    if(image.isNull())
        throw Exception(tr("The resource image is not supported or corrupted: %1.").arg(info.fileName()));

    return image;
}

void GridRenderer::onStart()
{
    try
    {
        QImage img = execute();
        sendResult(img);
        sendFinish();
    }
    catch(ThreadStoppedException&)
    {
        sendFinish();
    }
    catch(Exception& e)
    {
        sendError(e.what());
        sendFinish();
    }
}

QImage GridRenderer::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The renderer is already working."));

        stopped = false;
        actuallyStopped = false;

        if(grid->getHeight() == 0)
            throw IllegalArgumentException(tr("The grid is empty."));

        int width = grid->getWidth();
        for(QList<Cell>& list: grid->getCells())
            if(list.size() != width)
                throw IllegalArgumentException(tr("The grid is not rectangular."));

        sendLog(tr("Preparing the canvas for rendering."));

        QImage res(imageSize * grid->getWidth(), imageSize * grid->getHeight(), QImage::Format_ARGB32);
        res.fill(Qt::white);

        sendLog(tr("Starting."));

        for(QList<Cell>& list: grid->getCells())
        {
            sendLog(tr("Rendering grid row %1 of %2.").arg(QString::number(currentY + 1), QString::number(grid->getHeight())));

            for(Cell cell: list)
            {
                if(stopped)
                    throw ThreadStoppedException(tr("Renderer is stopped."));

                renderCell(res, cell);
                currentX ++;
            }

            currentX = 0;
            currentY ++;
        }

        clear();

        stopped = true;
        actuallyStopped = true;

        return res;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void GridRenderer::clear()
{
    renderedIndexes.clear();
    currentX = 0;
    currentY = 0;
}

void GridRenderer::renderCell(QImage& image, Cell cell)
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
            renderElement(image, cell);
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
    QImage tile;

    if(currentY < grid->getHeight()-1 && grid->getCells()[currentY+1][currentX].getType() == CellType::Element)
        tile = getImageFromCache("pin_vertical");
    else if(currentX > 0 && grid->getCells()[currentY][currentX-1].getType() == CellType::Element)
        tile = rotateImage(getImageFromCache("pin_horizontal"), 180);
    else if(currentY > 0 && grid->getCells()[currentY-1][currentX].getType() == CellType::Element)
        tile = rotateImage(getImageFromCache("pin_vertical"), 180);
    else
        tile = getImageFromCache("pin_horizontal");

    renderTileOnCurrentPosition(image, tile);
}

void GridRenderer::renderElement(QImage& image, Cell cell)
{
    std::bitset<4> nearbyElements(0); // top, bottom, left, right
    if(currentY > 0 && grid->getCells()[currentY-1][currentX].getType() == CellType::Element)
        nearbyElements[3] = 1;
    if(currentY < grid->getHeight()-1 && grid->getCells()[currentY+1][currentX].getType() == CellType::Element)
        nearbyElements[2] = 1;
    if(currentX > 0 && grid->getCells()[currentY][currentX-1].getType() == CellType::Element)
        nearbyElements[1] = 1;
    if(currentX < grid->getWidth()-1 && grid->getCells()[currentY][currentX+1].getType() == CellType::Element)
        nearbyElements[0] = 1;

    ulong res = nearbyElements.to_ulong();

    if(!renderedIndexes.contains(cell.getIndex()))
        renderAlias(image, cell);

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

void GridRenderer::renderAlias(QImage& image, Cell cell)
{
    if(currentX <= 0 || currentY <= 0)
        return;

    QString alias = lookupAlias(cell.getIndex());

    if(alias.isEmpty())
        return;

    for(int i=0; i<alias.length(); i++)
    {
        QPoint position((currentX-1 + i) * imageSize, (currentY - 1) * imageSize);
        QString tileName;

        QChar c = alias[i];

        if(c.isDigit())
            tileName = QString("digit_%1").arg(c.toLower());
        else if(c.isLower())
            tileName = QString("lower_%1").arg(c.toLower());
        else
            tileName = QString("upper_%1").arg(c.toLower());

        QImage tile = getImageFromCache(tileName);

        renderTileOnPosition(image, tile, position);
    }

    renderedIndexes.append(cell.getIndex());
}

QString GridRenderer::lookupAlias(qint64 index)
{
    for(SchemeElement el: scheme->getElements())
    {
        if(el.getIndex() == index)
        {
           return el.getAlias();
        }
    }

    return "";
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
            tile = getImageFromCache("lr");
            break;
        }
        case CellType::UL:
        {
            tile = getImageFromCache("dr");
            tile = rotateImage(tile, 180);
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
            tile = getImageFromCache("dr");
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
            tile = getImageFromCache("lru");
            break;
        }
        case CellType::LRD:
        {
            tile = getImageFromCache("lru");
            tile = rotateImage(tile, 180);
            break;
        }
        case CellType::UDLR:
        {
            tile = getImageFromCache("udlr");
            break;
        }
        case CellType::UDLRI:
        {
            tile = getImageFromCache("udlri");
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
        throw Exception(tr("Missing cache image: %1.").arg(key));

    return imageCache.value(key);
}

void GridRenderer::renderTileOnCurrentPosition(QImage& image, QImage tile)
{
    QPoint position(currentX * imageSize, currentY * imageSize);
    renderTileOnPosition(image, tile, position);
}

void GridRenderer::renderTileOnPosition(QImage& image, QImage tile, QPoint position)
{
    QPainter painter(&image);
    painter.drawImage(position, tile);
    painter.end();
}

QImage GridRenderer::rotateImage(QImage image, int degrees)
{
    if(degrees % 90 != 0)
        throw IllegalArgumentException(tr("Degrees value must be multiple of 90, passed: %1.").arg(QString::number(degrees)));

    QTransform transform;
    transform.rotate(degrees);

    return image.transformed(transform);
}
