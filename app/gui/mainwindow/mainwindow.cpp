#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->nodesTable->setModel(new NodeViewModel(ui->nodesTable));
    ui->nodesTable->horizontalHeader()->setStretchLastSection(true);
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
