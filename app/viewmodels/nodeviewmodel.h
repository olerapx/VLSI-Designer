#pragma once

#include <QAbstractTableModel>
#include <QMap>

#include "entitystatusdelegate.h"
#include "control/pool/manager/poolmanager.h"

static const QMap<EntityStatus, QString> nodeStatusMap
{
    { EntityStatus::Ready, QT_TRANSLATE_NOOP("NodeViewModel", "Ready") },
    { EntityStatus::Unconnected, QT_TRANSLATE_NOOP("NodeViewModel", "Unconnected") },
    { EntityStatus::NotResponding, QT_TRANSLATE_NOOP("NodeViewModel", "Not responding") },
    { EntityStatus::Initialization, QT_TRANSLATE_NOOP("NodeViewModel", "Initialization") },
    { EntityStatus::Assigned, QT_TRANSLATE_NOOP("NodeViewModel", "Assigned") },
    { EntityStatus::Working, QT_TRANSLATE_NOOP("NodeViewModel", "Working") },
    { EntityStatus::Error, QT_TRANSLATE_NOOP("NodeViewModel", "Error") },
    { EntityStatus::Incompatible, QT_TRANSLATE_NOOP("NodeViewModel", "Incompatible") },
    { EntityStatus::Manager, QT_TRANSLATE_NOOP("NodeViewModel", "Manager") },
    { EntityStatus::Node, QT_TRANSLATE_NOOP("NodeViewModel", "Node") }
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
    void onClearEntitiesInfo();
    void onUpdateEntityInfo(PoolEntityInfo& info);
    void onRemoveEntityInfo(int index);
    void onAddEntityInfo();

private:
    PoolEntity& entity;
};
