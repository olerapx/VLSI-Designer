#include "nodestatusdelegate.h"

NodeStatusDelegate::NodeStatusDelegate(QWidget* parent) :
    QStyledItemDelegate(parent)
{
    fillCache();
}

void NodeStatusDelegate::fillCache()
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

QImage NodeStatusDelegate::getImageFromCache(QString key) const
{
    if(imageCache.contains(key))
        return imageCache[key];

    return imageCache["empty"];
}

void NodeStatusDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if(index.column() != 1)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    NodeStatus status = qvariant_cast<NodeStatus>(index.data());

    QImage image;

    switch(status)
    {
    case NodeStatus::Ready:
        image = getImageFromCache("green");
        break;
    case NodeStatus::Unconnected:
        image = getImageFromCache("grey");
        break;
    case NodeStatus::NotResponding:
        image = getImageFromCache("red");
        break;
    case NodeStatus::Connecting:
        image = getImageFromCache("blue");
        break;
    case NodeStatus::Initialization:
        image = getImageFromCache("blue");
        break;
    case NodeStatus::Assigned:
        image = getImageFromCache("blue");
        break;
    case NodeStatus::Working:
        image = getImageFromCache("yellow");
        break;
    case NodeStatus::Error:
        image = getImageFromCache("red");
        break;
    case NodeStatus::Incompatible:
        image = getImageFromCache("red");
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
