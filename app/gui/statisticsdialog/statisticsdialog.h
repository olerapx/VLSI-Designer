#pragma once

#include <QDialog>

#include "datamodels/statistics/statistics.h"
#include "datamodels/grid/grid.h"
#include "control/filesystem/filesystem.h"

namespace Ui
{
    class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    StatisticsDialog(Statistics* statistics, Grid* grid, FileSystem& system, QWidget* parent = 0);
    ~StatisticsDialog();

private:
    void display();
    void showHeader();
    void showCommon();

    void showDataForLevel(int level);
    void showAverageData(int level);
    void showHostData(int level, int index);

    int getTotalTime();
    double getTotalTimeInSeconds();
    QString getModel();

    Ui::StatisticsDialog *ui;

    Statistics* statistics;
    Grid* grid;
    FileSystem& fileSystem;
};
