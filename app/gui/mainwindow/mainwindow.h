#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QMenu>

#include "app.h"
#include "config/configbuilder.h"
#include "control/pool/manager/poolmanager.h"
#include "control/pool/node/poolnode.h"
#include "network/networkscanner.h"
#include "viewmodels/nodeviewmodel.h"
#include "gui/networkconfigurationwindow/networkconfigurationdialog.h"
#include "gui/generatorwindow/generatorwindow.h"
#include "gui/addnodesdialog/addnodesdialog.h"
#include "gui/setupsessiondialog/setupsessiondialog.h"
#include "gui/statisticsdialog/statisticsdialog.h"

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
    void on_startButton_clicked();
    void on_stopButton_clicked();

    void onTableContextMenuRequested(QPoint pos);
    void onRemove();
    void onReconnect();

    void onFitTableToContent();

    void onSendManagerLog(QString log, LogType type);
    void onSendNodeLog(QString log, LogType type);

    void onSendManagerError(QString error);
    void onSendNodeError(QString error);

    void onDisableManager();
    void onEnableManager();
    void onFinish(Statistics* statistics);

private:
    void changeNetworkConfig(bool firstTime);
    bool tryChangeNetworkConfig(bool firstTime);

    void colorizeLog(QString& string, LogType type);
    void prependCurrentTime(QString& string);
    void createTableContextMenu();

    Ui::MainWindow *ui;
    QMenu tableContextMenu;
    QModelIndex lastContextActionIndex;

    Config config;
    FileSystem fileSystem;

    PoolManager manager;
    PoolNode node;    

    NetworkScanner scanner;

    NodeViewModel* nodeViewModel;    
};
