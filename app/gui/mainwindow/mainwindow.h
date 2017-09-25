#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMenu>

#include "app.h"
#include "config/configbuilder.h"
#include "control/manager/poolmanager.h"
#include "control/node/poolnode.h"
#include "network/networkscanner.h"
#include "viewmodels/nodeviewmodel.h"
#include "gui/networkconfigurationwindow/networkconfigurationdialog.h"
#include "gui/generatorwindow/generatorwindow.h"
#include "gui/addnodesdialog/addnodesdialog.h"
#include "gui/setupsessiondialog/setupsessiondialog.h"

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
    void on_addNodesButton_clicked();
    void on_setupButton_clicked();

    void onTableContextMenuRequested(QPoint pos);
    void onRemove();
    void onReconnect();

    void onSendManagerLog(QString log);
    void onSendNodeLog(QString log);    

private:
    void changeNetworkConfig(bool firstTime);
    bool tryChangeNetworkConfig(bool firstTime);

    void prependCurrentTime(QString& string);
    void createTableContextMenu();

    Ui::MainWindow *ui;
    QMenu tableContextMenu;
    QModelIndex lastContextActionIndex;

    Config config;

    PoolManager manager;
    PoolNode node;    

    NetworkScanner scanner;

    NodeViewModel* nodeViewModel;    
};
