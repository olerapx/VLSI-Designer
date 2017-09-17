#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
manager.getPoolNodes().append(PoolNodeInfo("asda", QHostAddress::LocalHost)); // TODO: temp, test
    ui->nodesTable->setModel(new NodeViewModel(ui->nodesTable, manager));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generatorAction_triggered()
{
    GeneratorWindow* window = new GeneratorWindow(this);
    window->show();
}

void MainWindow::on_addNodesButton_clicked()
{
    AddNodesDialog dialog(manager);
    if(dialog.exec())
    {
        manager.getPoolNodes().append(dialog.getSelectedNodes());
        // TODO: UPDATE TABLE
    }
}
