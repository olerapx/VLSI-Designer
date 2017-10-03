#include "entitystatusdelegate.h"

EntityStatusDelegate::EntityStatusDelegate(QWidget* parent) :
    QStyledItemDelegate(parent)
{
    fillCache();
}

void EntityStatusDelegate::fillCache()
{
    QString path = ":/resources/images/status";

    for(QFileInfo& info: QDir(path).entryInfoList())
    {
        QFile f(info.absoluteFilePath());

        f.open(QIODevice::ReadOnly);
        QImage image = QImage::fromData(f.readAll(), info.suffix().toLocal8Bit());
        f.close();

        imageCache.insert(info.completeBaseName(), image);
    }
}

QImage EntityStatusDelegate::getImageFromCache(QString key) const
{
    if(imageCache.contains(key))
        return imageCache[key];

    return imageCache["empty"];
}

void EntityStatusDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if(index.column() != 1)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    EntityStatus status = qvariant_cast<EntityStatus>(index.data());

    QImage image;

    switch(status)
    {
    case EntityStatus::Ready:
        image = getImageFromCache("green");
        break;
    case EntityStatus::Unconnected:
        image = getImageFromCache("grey");
        break;
    case EntityStatus::NotResponding:
        image = getImageFromCache("red");
        break;
    case EntityStatus::Connecting:
        image = getImageFromCache("blue");
        break;
    case EntityStatus::Initialization:
        image = getImageFromCache("blue");
        break;
    case EntityStatus::Assigned:
        image = getImageFromCache("blue");
        break;
    case EntityStatus::Working:
        image = getImageFromCache("yellow");
        break;
    case EntityStatus::Error:
        image = getImageFromCache("red");
        break;
    case EntityStatus::Incompatible:
        image = getImageFromCache("red");
        break;
    case EntityStatus::Manager:
        image = getImageFromCache("green");
        break;
    case EntityStatus::Node:
        image = getImageFromCache("blue");
        break;
    default:
        image = getImageFromCache("empty");
        break;
    }

    QPixmap pixmap = QPixmap::fromImage(image);

    QRect rect = option.rect;
    QPoint shift = QPoint((rect.width() - pixmap.width()) / 2, (rect.height() - pixmap.height()) / 2);

    painter->drawPixmap(rect.topLeft() + shift, pixmap);
}
