#pragma once

#include <QDialog>
#include <QNetworkInterface>

#include "control/manager/poolmanager.h"
#include "viewmodels/addnodeviewmodel.h"
#include "network/networkscanner.h"

namespace Ui
{
    class AddNodesDialog;
}
// TODO: on close - set manager parameters
class AddNodesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNodesDialog(PoolManager& manager, QWidget* parent = 0);
    ~AddNodesDialog();

    QList<PoolNodeInfo>& getSelectedNodes() { return selectedNodes; }

private slots:
    void on_scanButton_clicked();
    void on_ipv4Radio_toggled(bool checked);

    void onSendLog(QString data);
    void onSendAddress(QHostAddress senderHost, QString hostName);

    bool hasAddress(QHostAddress address);

private:
    void fillNetworkInterfaces();

    Ui::AddNodesDialog *ui;
    PoolManager& manager;

    NetworkScanner scanner;

    AddNodeViewModel* addNodeViewModel;

    QList<PoolNodeInfo> foundNodes;
    QList<PoolNodeInfo> selectedNodes;
    QList<QNetworkInterface> interfaces;
};
