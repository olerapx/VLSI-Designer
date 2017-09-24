#pragma once

#include <QAbstractTableModel>
#include <QMap>

#include "control/manager/poolmanager.h"

static const QMap<NodeStatus, QString> nodeStatusMap
{
    { NodeStatus::Ready, QT_TR_NOOP("Ready") },
    { NodeStatus::Unconnected, QT_TR_NOOP("Unconnected") },
    { NodeStatus::NotResponding, QT_TR_NOOP("Not responding") },
    { NodeStatus::Initialization, QT_TR_NOOP("Initialization") },
    { NodeStatus::Assigned, QT_TR_NOOP("Assigned") },
    { NodeStatus::Working, QT_TR_NOOP("Working") },
    { NodeStatus::Error, QT_TR_NOOP("Error") },
    { NodeStatus::Incompatible, QT_TR_NOOP("Incompatible") }
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

private slots:
    void onUpdateNodeInfo(PoolNodeInfo& info);
    void onRemoveNodeInfo(int index);

private:
    PoolManager& manager;
};
