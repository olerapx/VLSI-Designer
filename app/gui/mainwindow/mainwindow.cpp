#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    config(ConfigBuilder::readConfig()),
    scanner(config.getNodeTcpPort())
{
    ui->setupUi(this);

    nodeViewModel = new NodeViewModel(this, manager);
    ui->nodesTable->setModel(nodeViewModel);
    createTableContextMenu();

    connect(&manager, &PoolManager::sendLog, this, &MainWindow::onSendManagerLog);
    connect(&node, &PoolNode::sendLog, this, &MainWindow::onSendNodeLog);
    connect(ui->nodesTable, &QTableView::customContextMenuRequested, this, &MainWindow::onTableContextMenuRequested);
}

MainWindow::~MainWindow()
{
    disconnect(&manager, &PoolManager::sendLog, this, &MainWindow::onSendManagerLog);
    disconnect(&node, &PoolNode::sendLog, this, &MainWindow::onSendNodeLog);
    disconnect(ui->nodesTable, &QTableView::customContextMenuRequested, this, &MainWindow::onTableContextMenuRequested);

    delete ui;
}

void MainWindow::createTableContextMenu()
{
    tableContextMenu.addAction(tr("Remove"), this, &MainWindow::onRemove);
    tableContextMenu.addAction(tr("Reconnect"), this, &MainWindow::onReconnect);
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);

    QTimer::singleShot(50, this, &MainWindow::onShow);
}

void MainWindow::onShow()
{
    changeNetworkConfig(true);
}

void MainWindow::on_networkConfigurationAction_triggered()
{
    changeNetworkConfig(false);
}

void MainWindow::changeNetworkConfig(bool firstTime)
{
    bool success = false;

    manager.disable();
    node.disable();

    while(!success)
        success = tryChangeNetworkConfig(firstTime);

    manager.enable();
    node.enable();
}

bool MainWindow::tryChangeNetworkConfig(bool firstTime)
{
    try
    {
        NetworkConfigurationDialog dialog(config, manager, firstTime);

        if(dialog.exec() || firstTime)
        {
            manager.setSelfPort(config.getManagerTcpPort());
            node.setSelfPort(config.getNodeTcpPort());

            if(config.getMode() == Mode::IPv4)
                scanner.initIPv4Broadcast(config.getNetworkInterface(), config.getUdpPort());
            else
                scanner.initIPv6Multicast(config.getMulticastAddress(), config.getNetworkInterface(), config.getUdpPort());

        }
        return true;
    }
    catch(Exception& e)
    {
        QMessageBox::critical(this, tr("Network error"), tr("A network error detected while trying to initialize scanner:\n\n%1\n\n"
                                                            "Try set different parameters.").arg(e.what()));

    }

    return false;
}

void MainWindow::on_generatorAction_triggered()
{
    GeneratorWindow* window = new GeneratorWindow(config, this);
    window->show();
}

void MainWindow::on_addNodesButton_clicked()
{
    AddNodesDialog dialog(manager, scanner);
    if(dialog.exec())
    {
        nodeViewModel->appendRows(dialog.getSelectedNodes());
        manager.connectToUnconnectedNodes();
    }
}

void MainWindow::on_setupButton_clicked()
{
    SetupSessionDialog dialog(config, manager);
    if(dialog.exec())
    {
        manager.setSessionData(new SessionData(dialog.getScheme(), dialog.getArchitecture(), dialog.getLibraries()));
        ui->startButton->setEnabled(true);
    }
}


void MainWindow::onTableContextMenuRequested(QPoint pos)
{
    QModelIndex index = ui->nodesTable->indexAt(pos);

    if(index.row() == -1 || index.column() == -1)
        return;

    lastContextActionIndex = index;

    for(QAction* a: tableContextMenu.actions())
        a->setEnabled(true);

    PoolNodeInfo& info = manager.getPoolNodesInfo()[index.row()];

    if(info.getStatus() == NodeStatus::NotResponding || info.getStatus() == NodeStatus::Unconnected)
        tableContextMenu.actions()[0]->setEnabled(false);

    if(info.getStatus() != NodeStatus::NotResponding && info.getStatus() != NodeStatus::Unconnected)
        tableContextMenu.actions()[1]->setEnabled(false);

    tableContextMenu.popup(ui->nodesTable->viewport()->mapToGlobal(pos));
}

void MainWindow::onRemove()
{
    PoolNodeInfo& info = manager.getPoolNodesInfo()[lastContextActionIndex.row()];
    manager.removeNode(info);
}

void MainWindow::onReconnect()
{
    PoolNodeInfo& info = manager.getPoolNodesInfo()[lastContextActionIndex.row()];
    manager.connectToNode(info);
}

void MainWindow::onSendManagerLog(QString log)
{
    prependCurrentTime(log);
    ui->managerLogText->append(log);
}

void MainWindow::onSendNodeLog(QString log)
{
    prependCurrentTime(log);
    ui->nodeLogText->append(log);
}

void MainWindow::prependCurrentTime(QString& string)
{
    QString time = QString("[%1] ").arg(QTime::currentTime().toString("hh:mm:ss.zzz"));
    string.prepend(time);
}
