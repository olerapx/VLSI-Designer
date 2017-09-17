#include "addnodeviewmodel.h"

AddNodeViewModel::AddNodeViewModel(QObject* parent, QList<PoolNodeInfo>& nodes, int port)
    : QAbstractTableModel(parent),
      nodes(nodes),
      port(port)
{
}

int AddNodeViewModel::rowCount(const QModelIndex&) const
{
    return nodes.size();
}

int AddNodeViewModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant AddNodeViewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        PoolNodeInfo& info = nodes[row];

        switch (col)
        {
        case 0:
            return info.getHostName();
        case 1:
            return QString("%1:%2").arg(info.getAddress().toString(), QString::number(port));// TODO use info's port
        }
    }
    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return QVariant();
}

QVariant AddNodeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
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
                return QString(tr("Address"));
            }
        }
        return QVariant(section + 1);
    }
    return QVariant();
}

void AddNodeViewModel::appendRow(PoolNodeInfo info, const QModelIndex& parent)
{
    beginInsertRows(parent, nodes.size(), nodes.size());

    nodes.append(info);

    endInsertRows();
}

void AddNodeViewModel::clear()
{
    beginResetModel();

    nodes.clear();

    endResetModel();
}
