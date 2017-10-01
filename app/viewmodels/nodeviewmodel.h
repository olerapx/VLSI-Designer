#pragma once

#include <QAbstractTableModel>
#include <QMap>

#include "nodestatusdelegate.h"
#include "control/pool/manager/poolmanager.h"

class NodeViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    NodeViewModel(QObject* parent, PoolManager& manager);

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
    PoolManager& manager;
};
