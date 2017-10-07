#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

StatisticsDialog::StatisticsDialog(Statistics* statistics, FileSystem& system, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog),
    statistics(statistics),
    fileSystem(system)
{
    ui->setupUi(this);
    display();
}

StatisticsDialog::~StatisticsDialog()
{
    delete ui;
}

void StatisticsDialog::display()
{
    showHeader();

    for(int i=0; i<statistics->getData().size(); i++)
        showDataForLevel(i);
}

void StatisticsDialog::showHeader()
{
    ui->textBrowser->append(QString("<h1 style=\"text-align: center; text-transform: uppercase;\">%1</h1>"
                                    "<p style=\"text-align: center; color: #66e0ff; font-weight: bold\">%2</p>")
                                    .arg(tr("Session report"), fileSystem.getSessionName()));
}

void StatisticsDialog::showDataForLevel(int level)
{
    if(level != 0)
        showAverageData(level);

    for(int i=0; i<statistics->getData()[level].size(); i++)
        showHostData(level, i);
}

void StatisticsDialog::showAverageData(int level)
{
    ui->textBrowser->append(QString("<hr><br><h2 style=\"color: #33ccff; text-align: center; text-transform: uppercase;\">%1 %2</h2>"
                                    "<h3 style=\"color: #33ccff;\">%3</h3>"
                                    "<table cellpadding=\"5\" style=\"font-size: 10pt;\">"
                                    "<tr><td>%4</td> <td>%5 ms</td></tr>"
                                    "<tr><td>%6</td> <td>%7 ms</td></tr>"
                                    "<tr><td>%8</td> <td>%9 ms</td></tr>"
                                    "<tr><td>%10</td> <td>%11 ms</td></tr>"
                                    "<tr></tr>"
                                    "<tr><td>%12</td> <td>%13 ms</td></tr>"
                                    "<tr><td>%14</td> <td>%15 ms</td></tr>"
                                    "<tr><td>%16</td> <td>%17 ms</td></tr>"
                                    "<tr><td>%18</td> <td>%19 ms</td></tr>"
                                    "<tr></tr>"
                                    "<tr><td>%20</td> <td>%21%</td></tr>"
                                    "<tr><td>%22</td> <td>%23%</td></tr></table><br>")
                            .arg(tr("Level"), QString::number(level),
                                 tr("Average"),
                                 tr("Primary placement time"), QString::number(statistics->getAveragePrimaryPlacementTime(level)),
                                 tr("Secondary placement time"), QString::number(statistics->getAverageSecondaryPlacementTime(level)),
                                 tr("Internal routing time"), QString::number(statistics->getAverageInnerRoutingTime(level)))
                             .arg(tr("Total"), QString::number(statistics->getAverageTotalInnerTime(level)),
                                 tr("Decomposition time"), QString::number(statistics->getAverageDecompositionTime(level)),
                                 tr("Composition time"), QString::number(statistics->getAverageCompositionTime(level)),
                                 tr("External routing time"), QString::number(statistics->getAverageOuterRoutingTime(level)))
                              .arg(tr("Total"), QString::number(statistics->getAverageTotalOuterTime(level)),
                                 tr("Internal routed wires percent"), QString::number(statistics->getAverageInnerRoutedWiresPercent(level), 'g', 3),
                                 tr("Routed wires percent"), QString::number(statistics->getAverageRoutedWiresPercent(level), 'g', 3)));
}

void StatisticsDialog::showHostData(int level, int index)
{
    StatisticsEntry entry = statistics->getData()[level][index];

    ui->textBrowser->append(QString("<h3 style=\"color: #33ccff;\">%1 %2: %3</h3>"
                                    "<table cellpadding=\"5\" style=\"font-size: 10pt;\">"
                                    "<tr><td>%4</td> <td>%5 ms</td></tr>"
                                    "<tr><td>%6</td> <td>%7 ms</td></tr>"
                                    "<tr><td>%8</td> <td>%9 ms</td></tr>"
                                    "<tr><td>%10</td> <td>%11 ms</td></tr>"
                                    "<tr></tr>"
                                    "<tr><td>%12</td> <td>%13 ms</td></tr>"
                                    "<tr><td>%14</td> <td>%15 ms</td></tr>"
                                    "<tr><td>%16</td> <td>%17 ms</td></tr>"
                                    "<tr><td>%18</td> <td>%19 ms</td></tr>"
                                    "<tr></tr>"
                                    "<tr><td>%20</td> <td>%21 ms</td></tr>"
                                    "<tr><td>%22</td> <td>%23 ms</td></tr>"
                                    "<tr><td>%24</td> <td>%25%</td></tr>"
                                    "<tr><td>%26</td> <td>%27%</td></tr>"
                                    "<tr><td>%28</td> <td>%29%</td></tr></table><br>")
                            .arg(tr("Computer"), QString::number(index + 1), entry.getHostName(),
                                 tr("Primary placement time"), QString::number(entry.getPrimaryPlacememtTime()),
                                 tr("Secondary placement time"), QString::number(entry.getSecondaryPlacementTime()),
                                 tr("Internal routing time"), QString::number(entry.getInnerRoutingTime()))
                             .arg(tr("Total"), QString::number(entry.getTotalInnerTime()),
                                 tr("Decomposition time"), QString::number(entry.getDecompositionTime()),
                                 tr("Composition time"), QString::number(entry.getCompositionTime()),
                                 tr("External routing time"), QString::number(entry.getOuterRoutingTime()))
                              .arg(tr("Total"), QString::number(entry.getTotalOuterTime()),
                                 tr("Internal wires number"), QString::number(entry.getInnerWiresNumber()),
                                 tr("Wires number"), QString::number(entry.getWiresNumber()),
                                 tr("External wires percent"), QString::number(entry.getOuterWiresPercent()))
                              .arg(tr("Internal routed wires percent"), QString::number(entry.getInnerRoutedWiresPercent(), 'g', 3),
                                tr("Routed wires percent"), QString::number(entry.getRoutedWiresPercent(), 'g', 3)));

}
