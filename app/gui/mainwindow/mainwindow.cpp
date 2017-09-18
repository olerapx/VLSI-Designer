#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->nodesTable->setModel(new NodeViewModel(ui->nodesTable, manager));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);

    QTimer::singleShot(50, this, &MainWindow::onShow);
}

void MainWindow::onShow()
{
    on_networkConfigurationAction_triggered();
    initScanner();
}

void MainWindow::on_networkConfigurationAction_triggered()
{
    NetworkConfigurationDialog window(config, manager);
    window.exec();
}

void MainWindow::initScanner()
{//TODO: use one port from config
    //TODO: try-catch here
    if(config.getMode() == Mode::IPv4)
        scanner.initIPv4Broadcast(config.getNetworkInterface(), 40000, 40001);
    else
        scanner.initIPv6Multicast(config.getMulticastAddress(), config.getNetworkInterface(), 40000, 40001);
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
