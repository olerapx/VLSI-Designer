#pragma once

#include <QDialog>
#include <QNetworkInterface>

#include "control/manager/poolmanager.h"
#include "viewmodels/addnodeviewmodel.h"

namespace Ui
{
    class AddNodesDialog;
}
// TODO: on close - set manager parameters
class AddNodesDialog : public QDialog
{
    Q_OBJECT

public:
    AddNodesDialog(PoolManager& manager, NetworkScanner& scanner, QWidget* parent = 0);
    ~AddNodesDialog();

    QList<PoolNodeInfo>& getSelectedNodes() { return selectedNodes; }

private slots:
    void on_scanButton_clicked();

    void onSendLog(QString data);
    void onSendAddress(QHostAddress senderHost, QString hostName);

    bool hasAddress(QHostAddress address);

private:
    Ui::AddNodesDialog *ui;

    PoolManager& manager;
    NetworkScanner& scanner;

    AddNodeViewModel* addNodeViewModel;

    QList<PoolNodeInfo> foundNodes;
    QList<PoolNodeInfo> selectedNodes;
    QList<QNetworkInterface> interfaces;
};
