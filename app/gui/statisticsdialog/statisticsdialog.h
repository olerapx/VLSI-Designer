#pragma once

#include <QDialog>

#include "datamodels/statistics/statistics.h"
#include "control/filesystem/filesystem.h"

namespace Ui
{
    class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    StatisticsDialog(Statistics* statistics, FileSystem& system, QWidget* parent = 0);
    ~StatisticsDialog();

private:
    void display();
    void showHeader();

    void showDataForLevel(int level);
    void showAverageData(int level);
    void showHostData(int level, int index);

    Ui::StatisticsDialog *ui;

    Statistics* statistics;
    FileSystem& fileSystem;
};
