#include "nodeviewmodel.h"

NodeViewModel::NodeViewModel(QObject* parent, PoolManager& manager) :
    QAbstractTableModel(parent),
    manager(manager)
{
}

int NodeViewModel::rowCount(const QModelIndex&) const
{
    return manager.getPoolNodes().size();
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

        PoolNodeInfo& info = manager.getPoolNodes()[row];

        switch(col)
        {
        case 0:
            return info.getHostName();
        case 1:
            return nodeStatusMap.value(info.getStatus());
        case 2:
            return ((info.getProgramVersion() == 0.0)? "" : QVariant(info.getProgramVersion()));
        case 3:
            return QString("%1:%2").arg(info.getAddress().toString(), QString::number(manager.getPort())); // TODO use info's port
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
