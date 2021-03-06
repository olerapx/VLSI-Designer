#include "nodeviewmodel.h"

NodeViewModel::NodeViewModel(QObject* parent, PoolEntity& entity) :
    QAbstractTableModel(parent),
    entity(entity)
{
    connect(&entity, &PoolEntity::sendClearEntitiesInfo, this, &NodeViewModel::onClearEntitiesInfo);
    connect(&entity, &PoolEntity::sendUpdateEntityInfo, this, &NodeViewModel::onUpdateEntityInfo);
    connect(&entity, &PoolEntity::sendRemoveEntityInfo, this, &NodeViewModel::onRemoveEntityInfo);
    connect(&entity, &PoolEntity::sendAddEntityInfo, this, &NodeViewModel::onAddEntityInfo);
}

int NodeViewModel::rowCount(const QModelIndex&) const
{
    return entity.getKnownEntities().size();
}

int NodeViewModel::columnCount(const QModelIndex&) const
{
    return 5;
}

QVariant NodeViewModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    PoolEntityInfo& info = entity.getKnownEntities()[row];

    if(role == Qt::DisplayRole)
    {
        switch(col)
        {
        case 0:
            return info.getHostName();
        case 1:
        {
            QVariant var;
            var.setValue(info.getStatus());
            return var;
        }
        case 2:
            return tr(nodeStatusMap.value(info.getStatus()).toLocal8Bit().constData());
        case 3:
            return ((info.getProgramVersion() == Version()) ? "" : info.getProgramVersion().toString());
        case 4:
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
            switch(section)
            {
            case 0:
                return QString(tr("Hostname"));
            case 1:
                return QString("");
            case 2:
                return QString(tr("Status"));
            case 3:
                return QString(tr("Program version"));
            case 4:
                return QString(tr("Address"));
            }
        }
        return QVariant(section + 1);
    }
    return QVariant();
}

void NodeViewModel::appendRow(PoolEntityInfo info, const QModelIndex& parent)
{
    beginInsertRows(parent, entity.getKnownEntities().size(), entity.getKnownEntities().size());
    entity.getKnownEntities().append(info);
    endInsertRows();

    sendFitTableToContent();
}

void NodeViewModel::appendRows(QList<PoolEntityInfo>& list, const QModelIndex& parent)
{
    if(list.size() == 0)
        return;

    beginInsertRows(parent, entity.getKnownEntities().size(), entity.getKnownEntities().size() + list.size() - 1);
    entity.getKnownEntities().append(list);
    endInsertRows();

    sendFitTableToContent();
}

void NodeViewModel::onClearEntitiesInfo()
{
    beginResetModel();
    endResetModel();

    sendFitTableToContent();
}

void NodeViewModel::onUpdateEntityInfo(PoolEntityInfo& info)
{
    int rowIndex = entity.getKnownEntities().indexOf(info);

    dataChanged(createIndex(rowIndex, 0), createIndex(rowIndex, 3));
    sendFitTableToContent();
}

void NodeViewModel::onRemoveEntityInfo(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();

    sendFitTableToContent();
}

void NodeViewModel::onAddEntityInfo()
{
    beginInsertRows(QModelIndex(), entity.getKnownEntities().size() - 1, entity.getKnownEntities().size() - 1);
    endInsertRows();

    sendFitTableToContent();
}
