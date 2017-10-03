#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QImage>
#include <QFile>
#include <QDir>

#include "control/pool/poolentityinfo.h"

class EntityStatusDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    EntityStatusDelegate(QWidget* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    void fillCache();
    QImage getImageFromCache(QString key) const;

    QMap<QString, QImage> imageCache;
};
