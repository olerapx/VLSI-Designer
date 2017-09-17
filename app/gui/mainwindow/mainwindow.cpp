#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::readConfig()
{
    //TODO: read from file
    config.setMode(Mode::IPv6);
    config.setMulticastAddress(QHostAddress("FF02::1"));
    config.setPort(40000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);

    readConfig();

    manager.setPort(40000); //TODO: is it a bad idea to set parameters in manager here?
    manager.setUsedInterface(QNetworkInterface::allInterfaces()[0]);

    on_networkConfigurationAction_triggered();
    initScanner();

manager.getPoolNodes().append(PoolNodeInfo("asda", QHostAddress::LocalHost)); // TODO: temp, test
    ui->nodesTable->setModel(new NodeViewModel(ui->nodesTable, manager));
}

void MainWindow::on_networkConfigurationAction_triggered()
{
    NetworkConfigurationDialog window(config, manager);
    window.exec();
}

void MainWindow::initScanner()
{//TODO: use one port from config
    if(config.getMode() == Mode::IPv4)
        scanner.initIPv4Broadcast(*(manager.getUsedInterface()), 40000, 40001);
    else
        scanner.initIPv6Multicast(config.getMulticastAddress(), *(manager.getUsedInterface()), 40000, 40001);
}

void MainWindow::on_generatorAction_triggered()
{
    GeneratorWindow* window = new GeneratorWindow(this);
    window->show();
}

void MainWindow::on_addNodesButton_clicked()
{
    AddNodesDialog dialog(manager, scanner);
    if(dialog.exec())
    {
        manager.getPoolNodes().append(dialog.getSelectedNodes());
        // TODO: UPDATE TABLE
    }
}
