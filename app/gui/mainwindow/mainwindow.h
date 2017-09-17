#pragma once

#include <QMainWindow>

#include "control/manager/poolmanager.h"
#include "viewmodels/nodeviewmodel.h"
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
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void on_generatorAction_triggered();

    void on_addNodesButton_clicked();

private:
    Ui::MainWindow *ui;

    PoolManager manager;
};
