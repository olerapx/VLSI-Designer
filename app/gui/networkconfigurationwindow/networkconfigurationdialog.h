#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include "config/config.h"
#include "control/pool/manager/poolmanager.h"
#include "control/pool/node/poolnode.h"

namespace Ui
{
    class NetworkConfigurationWindow;
}

class NetworkConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    NetworkConfigurationDialog(Config& config, PoolManager& manager, PoolNode& node, bool firstTime = false, QWidget* parent = 0);
    void fillNetworkInterfaces();
    void readConfig();

    void disableIfManagerIsStarted();

    void writeConfig();

    ~NetworkConfigurationDialog();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_okButton_clicked();
    void on_ipv4Radio_toggled(bool checked);

private:
    void setValidators();

    Ui::NetworkConfigurationWindow* ui;

    Config& config;
    PoolManager& manager;
    PoolNode& node;

    QList<QNetworkInterface> interfaces;

    bool firstTime;
};
