#pragma once

#include <QMainWindow>

#include "viewmodels/nodeviewmodel.h"
#include "gui/generatorwindow/generatorwindow.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_generatorAction_triggered();

private:
    Ui::MainWindow *ui;
};
