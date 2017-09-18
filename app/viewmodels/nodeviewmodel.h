#pragma once

#include <QAbstractTableModel>
#include <QMap>

#include "control/manager/poolmanager.h"

static const QMap<NodeStatus, QString> nodeStatusMap
{
    { NodeStatus::Ready, QObject::tr("Ready") },
    { NodeStatus::Unconnected, QObject::tr("Unconnected") },
    { NodeStatus::NotResponding, QObject::tr("Not responding") },
    { NodeStatus::Initialization, QObject::tr("Initialization") },
    { NodeStatus::Assigned, QObject::tr("Assigned") },
    { NodeStatus::Working, QObject::tr("Working") },
    { NodeStatus::Error, QObject::tr("Error") },
    { NodeStatus::Incompatible, QObject::tr("Incompatible") }
};

class NodeViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    NodeViewModel(QObject* parent, PoolManager& manager);

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void appendRow(PoolNodeInfo info, const QModelIndex& parent = QModelIndex());
    void appendRows(QList<PoolNodeInfo>& list, const QModelIndex& parent = QModelIndex());

private:
    PoolManager& manager;
};
