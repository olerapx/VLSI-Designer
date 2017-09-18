#pragma once

#include <QDialog>

#include "config/config.h"
#include "control/manager/poolmanager.h"

namespace Ui
{
    class NetworkConfigurationWindow;
}

class NetworkConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    NetworkConfigurationDialog(Config& config, PoolManager& manager, QWidget* parent = 0);
    void fillNetworkInterfaces();
    void readConfig();

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

    QList<QNetworkInterface> interfaces;
};
