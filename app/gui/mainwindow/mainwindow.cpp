#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    showed(false),
    config(ConfigBuilder::readConfig()),
    fileSystem(config.getSessionsPath()),
    manager(App::APP_VERSION, fileSystem),
    node(App::APP_VERSION, fileSystem),
    scanner(config.getNodeTcpPort())
{
    ui->setupUi(this);

    nodeViewModel = new NodeViewModel(this, manager);    
    ui->nodesTable->setModel(nodeViewModel);
    ui->nodesTable->setItemDelegate(new EntityStatusDelegate(ui->nodesTable));
    ui->nodesTable->resizeColumnsToContents();
    createTableContextMenu();

    connect(nodeViewModel, &NodeViewModel::sendFitTableToContent, this, &MainWindow::onFitTableToContent);

    connect(&manager, &PoolEntity::sendLog, this, &MainWindow::onSendManagerLog, Qt::QueuedConnection);
    connect(&node, &PoolEntity::sendLog, this, &MainWindow::onSendNodeLog, Qt::QueuedConnection);
    connect(&manager, &PoolEntity::sendError, this, &MainWindow::onSendManagerError, Qt::QueuedConnection);
    connect(&node, &PoolEntity::sendError, this, &MainWindow::onSendNodeError, Qt::QueuedConnection);

    connect(&node, &PoolNode::sendDisableManager, this, &MainWindow::onDisableManager, Qt::QueuedConnection);
    connect(&node, &PoolNode::sendEnableManager, this, &MainWindow::onEnableManager, Qt::QueuedConnection);
    connect(&manager, &PoolManager::sendFinish, this, &MainWindow::onFinish, Qt::QueuedConnection);

    connect(ui->nodesTable, &QTableView::customContextMenuRequested, this, &MainWindow::onTableContextMenuRequested);
}

MainWindow::~MainWindow()
{
    disconnect(&manager, 0, 0, 0);
    disconnect(&node, 0, 0, 0);
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

    if(!showed)
    {
        QTimer::singleShot(50, this, &MainWindow::onShow);
        showed = true;
    }
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

    if(!manager.isStarted() && !node.isStarted())
    {
        manager.disable();
        node.disable();
    }

    while(!success)
        success = tryChangeNetworkConfig(firstTime);

    manager.enable();
    node.enable();
}

bool MainWindow::tryChangeNetworkConfig(bool firstTime)
{
    try
    {
        NetworkConfigurationDialog dialog(config, manager, node, firstTime);

        if(dialog.exec() || firstTime)
        {
            manager.setSelfPort(config.getManagerTcpPort());
            node.setSelfPort(config.getNodeTcpPort());

            if(config.getMode() == Mode::IPv4)
                scanner.initIPv4Broadcast(config.getNetworkInterface(), config.getUdpPort());
            else
                scanner.initIPv6Multicast(config.getMulticastAddress(), config.getNetworkInterface(), config.getUdpPort());

            manager.enable();
            node.enable();
        }
        return true;
    }
    catch(Exception& e)
    {
        QMessageBox::warning(this, tr("Network error"), tr("Cannot initialize network module:\n\n%1\n\n"
                                                            "Try set different parameters.").arg(e.what()));

        manager.disable();
        node.disable();
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

void MainWindow::on_startButton_clicked()
{
    if(manager.isStarted())
        return;

    if(manager.getKnownEntities().isEmpty())
    {
        QMessageBox::information(this, tr("No nodes added"), tr("Cannot start work without any node added. Add nodes and try again."));
        return;
    }

    ui->setupButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    manager.start();
}

void MainWindow::on_stopButton_clicked()
{
    onSendManagerLog(tr("Stopping the design process..."), LogType::Common);
    manager.stop();

    ui->setupButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void MainWindow::onTableContextMenuRequested(QPoint pos)
{
    QModelIndex index = ui->nodesTable->indexAt(pos);

    if(index.row() == -1 || index.column() == -1)
        return;

    lastContextActionIndex = index;

    for(QAction* a: tableContextMenu.actions())
        a->setEnabled(true);

    PoolEntityInfo& info = manager.getKnownEntities()[index.row()];

    if(manager.isStarted())
        tableContextMenu.actions()[0]->setEnabled(false);

    if(info.getStatus() != EntityStatus::NotResponding && info.getStatus() != EntityStatus::Unconnected)
        tableContextMenu.actions()[1]->setEnabled(false);

    tableContextMenu.popup(ui->nodesTable->viewport()->mapToGlobal(pos));
}

void MainWindow::onRemove()
{
    PoolEntityInfo& info = manager.getKnownEntities()[lastContextActionIndex.row()];
    manager.removeNode(info);
}

void MainWindow::onReconnect()
{
    PoolEntityInfo& info = manager.getKnownEntities()[lastContextActionIndex.row()];
    manager.connectToNode(info);
}

void MainWindow::onFitTableToContent()
{
    ui->nodesTable->resizeColumnsToContents();
    QRect rect = this->geometry();
    rect.setWidth(rect.width() - 1);
    this->setGeometry(rect);

    rect.setWidth(rect.width() + 1);
    this->setGeometry(rect);
}

void MainWindow::onSendManagerLog(QString log, LogType type)
{
    colorizeLog(log, type);
    prependCurrentTime(log);
    ui->managerLogText->append(log);
}

void MainWindow::onSendNodeLog(QString log, LogType type)
{
    colorizeLog(log, type);
    prependCurrentTime(log);
    ui->nodeLogText->append(log);
}

void MainWindow::colorizeLog(QString &string, LogType type)
{
    if(type == LogType::Common)
        return;

    switch(type)
    {
    case LogType::Success:
        string.prepend("<font color=\"green\">");
        string.append("<font/>");
        break;
    case LogType::Warning:
        string.prepend("<font color=\"#ff9900\">");
        string.append("<font/>");
        break;
    case LogType::Error:
        string.prepend("<font color=\"#ff3300\">");
        string.append("<font/>");
        break;
    default:
        return;
    }
}

void MainWindow::prependCurrentTime(QString& string)
{
    QString time = QString("[%1] ").arg(QTime::currentTime().toString("hh:mm:ss"));
    string.prepend(time);
}

void MainWindow::onSendManagerError(QString error)
{
    colorizeLog(error, LogType::Error);
    prependCurrentTime(error);
    ui->managerLogText->append(error);
}

void MainWindow::onSendNodeError(QString error)
{
    colorizeLog(error, LogType::Error);
    prependCurrentTime(error);
    ui->nodeLogText->append(error);
}

void MainWindow::onDisableManager()
{
    manager.disable();

    if(manager.isStarted())
        return;

    ui->managerWidget->setMaximumSize(QSize(0, 0));
    ui->logLayout->setSpacing(0);

    delete nodeViewModel;
    nodeViewModel = new NodeViewModel(this, node);
    ui->nodesTable->setModel(nodeViewModel);

    connect(nodeViewModel, &NodeViewModel::sendFitTableToContent, this, &MainWindow::onFitTableToContent);
    disconnect(ui->nodesTable, &QTableView::customContextMenuRequested, this, &MainWindow::onTableContextMenuRequested);
}

void MainWindow::onEnableManager()
{
    if(manager.isStarted())
        return;

    manager.enable();

    ui->managerWidget->setMaximumSize(QSize(16777215, 16777215));
    ui->logLayout->setSpacing(6);

    delete nodeViewModel;
    nodeViewModel = new NodeViewModel(this, manager);
    ui->nodesTable->setModel(nodeViewModel);

    connect(nodeViewModel, &NodeViewModel::sendFitTableToContent, this, &MainWindow::onFitTableToContent);
    connect(ui->nodesTable, &QTableView::customContextMenuRequested, this, &MainWindow::onTableContextMenuRequested);
}

void MainWindow::onFinish(Statistics* statistics, Grid* grid)
{
    ui->setupButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

    StatisticsDialog dialog(statistics, grid, fileSystem);
    dialog.exec();

    delete statistics;
    delete grid;
}
