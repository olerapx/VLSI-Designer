#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QImage>
#include <QFile>
#include <QDir>

#include "control/pool/poolentityinfo.h"

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
