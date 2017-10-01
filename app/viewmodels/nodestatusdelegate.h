#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QImage>
#include <QFile>
#include <QDir>

#include "control/pool/poolentityinfo.h"

static const QMap<NodeStatus, QString> nodeStatusMap
{
    { NodeStatus::Ready, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Ready") },
    { NodeStatus::Unconnected, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Unconnected") },
    { NodeStatus::NotResponding, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Not responding") },
    { NodeStatus::Initialization, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Initialization") },
    { NodeStatus::Assigned, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Assigned") },
    { NodeStatus::Working, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Working") },
    { NodeStatus::Error, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Error") },
    { NodeStatus::Incompatible, QT_TRANSLATE_NOOP("NodeStatusDelegate", "Incompatible") }
};

class NodeStatusDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    NodeStatusDelegate(QWidget* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    void fillCache();
    QImage getImageFromCache(QString key) const;

    QMap<QString, QImage> imageCache;
};
