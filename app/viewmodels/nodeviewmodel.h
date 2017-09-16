#pragma once

#include <QAbstractTableModel>

class NodeViewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    NodeViewModel(QObject* parent);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
