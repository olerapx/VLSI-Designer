#pragma once

#include <QDialog>
#include <QTime>
#include <QDesktopServices>

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

private slots:
    void on_openGridImageButton_clicked();

    void on_openSessionDirectoryButton_clicked();

private:
    void display();
    void showHeader();
    void showCommon();

    void appendRow(QString& data, QString key, QString value);
    void appendEmptyRow(QString& data);

    void showDataForLevel(int level);
    void showAverageData(int level);
    void showHostData(int level, int index);

    int getTotalTime();
    QString getTotalTimeString();
    QString getModel();

    Ui::StatisticsDialog *ui;

    Statistics* statistics;
    Grid* grid;
    FileSystem& fileSystem;
};
