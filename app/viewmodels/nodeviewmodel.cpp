#include "nodeviewmodel.h"

NodeViewModel::NodeViewModel(QObject* parent) :
    QAbstractTableModel(parent)
{

}

int NodeViewModel::rowCount(const QModelIndex& parent) const
{
    return 2;
}

int NodeViewModel::columnCount(const QModelIndex& parent) const
{
    return 4;
}

QVariant NodeViewModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

QVariant NodeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if(role == Qt::DisplayRole)
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
            return QString(tr("IP"));
        }
    }
    return QVariant();
}
