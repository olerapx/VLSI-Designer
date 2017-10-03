#pragma once

#include <QAbstractTableModel>
#include <QMap>

#include "nodestatusdelegate.h"
#include "control/pool/manager/poolmanager.h"

static const QMap<NodeStatus, QString> nodeStatusMap
{
    { NodeStatus::Ready, QT_TRANSLATE_NOOP("NodeViewModel", "Ready") },
    { NodeStatus::Unconnected, QT_TRANSLATE_NOOP("NodeViewModel", "Unconnected") },
    { NodeStatus::NotResponding, QT_TRANSLATE_NOOP("NodeViewModel", "Not responding") },
    { NodeStatus::Initialization, QT_TRANSLATE_NOOP("NodeViewModel", "Initialization") },
    { NodeStatus::Assigned, QT_TRANSLATE_NOOP("NodeViewModel", "Assigned") },
    { NodeStatus::Working, QT_TRANSLATE_NOOP("NodeViewModel", "Working") },
    { NodeStatus::Error, QT_TRANSLATE_NOOP("NodeViewModel", "Error") },
    { NodeStatus::Incompatible, QT_TRANSLATE_NOOP("NodeViewModel", "Incompatible") },
    { NodeStatus::Manager, QT_TRANSLATE_NOOP("NodeViewModel", "Manager") },
    { NodeStatus::Node, QT_TRANSLATE_NOOP("NodeViewModel", "Node") }
};

class NodeViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    NodeViewModel(QObject* parent, PoolEntity& entity);

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void appendRow(PoolEntityInfo info, const QModelIndex& parent = QModelIndex());
    void appendRows(QList<PoolEntityInfo>& list, const QModelIndex& parent = QModelIndex());

private slots:
    void onClearNodeInfo();
    void onUpdateNodeInfo(PoolEntityInfo& info);
    void onRemoveNodeInfo(int index);

private:
    PoolEntity& entity;
};
