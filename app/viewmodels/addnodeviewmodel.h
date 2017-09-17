#pragma once

#include <QAbstractTableModel>

#include "control/manager/poolnodeinfo.h"

class AddNodeViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AddNodeViewModel(QObject* parent, QList<PoolNodeInfo>& nodes, int port);

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void appendRow(PoolNodeInfo info, const QModelIndex& parent = QModelIndex());
    void clear();

private:
    QList<PoolNodeInfo>& nodes;
    int port;
};
