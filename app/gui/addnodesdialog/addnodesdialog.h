#pragma once

#include <QDialog>
#include <QNetworkInterface>

#include "network/networkscanner.h"
#include "control/manager/poolmanager.h"
#include "viewmodels/addnodeviewmodel.h"

namespace Ui
{
    class AddNodesDialog;
}

class AddNodesDialog : public QDialog
{
    Q_OBJECT

public:
    AddNodesDialog(PoolManager& manager, NetworkScanner& scanner, QWidget* parent = 0);
    ~AddNodesDialog();

    QList<PoolNodeInfo>& getSelectedNodes() { return selectedNodes; }

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_scanButton_clicked();
    void on_addSelectedButton_clicked();
    void on_addAllButton_clicked();

    void onSendLog(QString data);
    void onSendAddress(QHostAddress senderHost, QString hostName, int tcpPort);

private:
    bool hasAddress(QHostAddress address);

    Ui::AddNodesDialog *ui;

    PoolManager& manager;
    NetworkScanner& scanner;

    AddNodeViewModel* addNodeViewModel;

    QList<PoolNodeInfo> foundNodes;
    QList<PoolNodeInfo> selectedNodes;
    QList<QNetworkInterface> interfaces;
};
