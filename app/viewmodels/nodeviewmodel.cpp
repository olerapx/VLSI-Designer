#include "nodeviewmodel.h"

NodeViewModel::NodeViewModel(QObject* parent, PoolManager& manager) :
    QAbstractTableModel(parent),
    manager(manager)
{
    connect(&manager, &PoolManager::sendClearNodesInfo, this, &NodeViewModel::onClearNodeInfo);
    connect(&manager, &PoolManager::sendUpdateNodeInfo, this, &NodeViewModel::onUpdateNodeInfo);
    connect(&manager, &PoolManager::sendRemoveNodeInfo, this, &NodeViewModel::onRemoveNodeInfo);
}

int NodeViewModel::rowCount(const QModelIndex&) const
{
    return manager.getPoolNodesInfo().size();
}

int NodeViewModel::columnCount(const QModelIndex&) const
{
    return 4;
}

QVariant NodeViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        PoolEntityInfo& info = manager.getPoolNodesInfo()[row];

        switch(col)
        {
        case 0:
            return info.getHostName();
        case 1:
            return QObject::tr(nodeStatusMap.value(info.getStatus()).toLocal8Bit().constData());
        case 2:
            return ((info.getProgramVersion() == Version()) ? "" : info.getProgramVersion().toString());
        case 3:
            return QString("%1:%2").arg(info.getAddress().toString(), QString::number(info.getTcpPort()));
        }
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant NodeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString(tr("Hostname"));
            case 1:
                return QString(tr("Status"));
            case 2:
                return QString(tr("Program version"));
            case 3:
                return QString(tr("Address"));
            }
        }
        return QVariant(section + 1);
    }
    return QVariant();
}

void NodeViewModel::appendRow(PoolEntityInfo info, const QModelIndex& parent)
{
    beginInsertRows(parent, manager.getPoolNodesInfo().size(), manager.getPoolNodesInfo().size());

    manager.getPoolNodesInfo().append(info);

    endInsertRows();
}

void NodeViewModel::appendRows(QList<PoolEntityInfo>& list, const QModelIndex& parent)
{
    if(list.size() == 0)
        return;

    beginInsertRows(parent, manager.getPoolNodesInfo().size(), manager.getPoolNodesInfo().size() + list.size() - 1);

    manager.getPoolNodesInfo().append(list);

    endInsertRows();
}

void NodeViewModel::onClearNodeInfo()
{
    beginResetModel();
    endResetModel();
}

void NodeViewModel::onUpdateNodeInfo(PoolEntityInfo& info)
{
    int rowIndex = manager.getPoolNodesInfo().indexOf(info);
    dataChanged(createIndex(rowIndex, 0), createIndex(rowIndex, 3));
}

void NodeViewModel::onRemoveNodeInfo(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}
