#include <QtTest>
#include <QCoreApplication>
#include "util/renderer/gridrenderer.h"

class GridRendererTest : public QObject
{
    Q_OBJECT

public:
    GridRendererTest() {}
    ~GridRendererTest() {}

private slots:
    void renderTest();

private:
    QImage readImageFromFile(const QString filePath);
};

void GridRendererTest::renderTest()
{
    GridRenderer renderer;

    Grid* g = new Grid(10);

    QList<Cell> row1{Cell(CellType::Empty), Cell(CellType::Element, 0),
                Cell(CellType::Pin, 0 , "z"), Cell(CellType::DL)};
    QList<Cell> row2{Cell(CellType::Empty), Cell(CellType::DR),
                Cell(CellType::LR), Cell(CellType::UL)};
    QList<Cell> row3{Cell(CellType::Empty), Cell(CellType::UD),
                Cell(CellType::Pin, 1, "a1"), Cell(CellType::Element, 1)};
    QList<Cell> row4{Cell(CellType::Empty), Cell(CellType::UR),
                    Cell(CellType::Pin, 1, "a2"), Cell(CellType::Element, 1)};

    g->getCells().append(row1);
    g->getCells().append(row2);
    g->getCells().append(row3);
    g->getCells().append(row4);

    QImage img = renderer.render(g);

    delete g;
    img.save("111.png", "PNG");
}

QImage GridRendererTest::readImageFromFile(const QString filePath)
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

QTEST_MAIN(GridRendererTest)

#include "tst_gridrenderertest.moc"
