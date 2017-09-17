#ifndef NETWORKCONFIGURATIONWINDOW_H
#define NETWORKCONFIGURATIONWINDOW_H

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

private slots:
    void on_okButton_clicked();

    void on_ipv4Radio_toggled(bool checked);

private:
    Ui::NetworkConfigurationWindow* ui;

    Config& config;
    PoolManager& manager;

    QList<QNetworkInterface> interfaces;
};

#endif // NETWORKCONFIGURATIONWINDOW_H
