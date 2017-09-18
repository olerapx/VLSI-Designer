#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scanner(config.getTcpPort())
{
    ui->setupUi(this);

    nodeViewModel = new NodeViewModel(this, manager);
    ui->nodesTable->setModel(nodeViewModel);
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
    changeNetworkConfig(true);
}

void MainWindow::on_networkConfigurationAction_triggered()
{
    changeNetworkConfig(false);
}

void MainWindow::changeNetworkConfig(bool firstTime)
{
    bool success = false;

    while(!success)
    {
        try
        {
            NetworkConfigurationDialog dialog(config, manager, firstTime);

            if(dialog.exec() || firstTime)
            {
                manager.setPort(config.getTcpPort());

                if(config.getMode() == Mode::IPv4)
                    scanner.initIPv4Broadcast(config.getNetworkInterface(), config.getUdpPort());
                else
                    scanner.initIPv6Multicast(config.getMulticastAddress(), config.getNetworkInterface(), config.getUdpPort());

            }
            success = true;
        }
        catch(Exception& e)
        {
            QMessageBox::critical(this, tr("Network error"), tr("A network error detected while trying to initialize scanner:\n\n%1\n\n"
                                                         "Try set different parameters.").arg(e.what()));

        }
    }
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
        nodeViewModel->appendRows(dialog.getSelectedNodes());
    }
}
