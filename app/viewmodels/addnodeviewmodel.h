#pragma once

#include <QAbstractTableModel>

#include "control/pool/poolentityinfo.h"

class AddNodeViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AddNodeViewModel(QObject* parent, QList<PoolEntityInfo>& nodes);

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void appendRow(PoolEntityInfo info, const QModelIndex& parent = QModelIndex());
    void clear();

private:
    QList<PoolEntityInfo>& nodes;
};
