#pragma once

#include <QMainWindow>
#include <QTimer>

#include "config/config.h"
#include "control/manager/poolmanager.h"
#include "network/networkscanner.h"
#include "viewmodels/nodeviewmodel.h"
#include "gui/networkconfigurationwindow/networkconfigurationdialog.h"
#include "gui/generatorwindow/generatorwindow.h"
#include "gui/addnodesdialog/addnodesdialog.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

protected:
    void showEvent(QShowEvent* event);

private slots:
    void onShow();

    void on_generatorAction_triggered();
    void on_networkConfigurationAction_triggered();
    void changeNetworkConfig(bool firstTime);

    void on_addNodesButton_clicked();

private:
    Ui::MainWindow *ui;

    PoolManager manager;
    NetworkScanner scanner;

    NodeViewModel* nodeViewModel;

    Config config;
};
