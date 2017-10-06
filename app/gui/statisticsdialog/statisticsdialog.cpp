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
    showAverageData(level);

    for(int i=0; i<statistics->getData()[level].size(); i++)
        showHostData(level, i);
}

void StatisticsDialog::showAverageData(int level)
{
    ui->textBrowser->append(QString("<hr><br><h2 style=\"color: #33ccff; text-align: center; text-transform: uppercase;\">%1 %2</h2>"
                                    "<h3 style=\"color: #33ccff;\">%3</h3>"
                                    "<span style=\"font-size: 10pt;\">"
                                    "%4: %5ms<br>"
                                    "%6: %7ms<br>"
                                    "%8: %9ms<br>"
                                    "%10: %11ms<br>"
                                    "<br>"
                                    "%12: %13ms<br>"
                                    "%14: %15ms<br>"
                                    "%16: %17ms<br>"
                                    "%18: %19ms<br>"
                                    "<br>"
                                    "%20: %21%<br>"
                                    "%22: %23%<br></span>")
                            .arg(tr("Level"), QString::number(level + 1),
                                 tr("Average"),
                                 tr("Primary placement time"), QString::number(statistics->getAveragePrimaryPlacementTime(level)),
                                 tr("Secondary placement time"), QString::number(statistics->getAverageSecondaryPlacementTime(level)),
                                 tr("Inner routing time"), QString::number(statistics->getAverageInnerRoutingTime(level)))
                             .arg(tr("Total"), QString::number(statistics->getAverageTotalInnerTime(level)),
                                 tr("Decomposition time"), QString::number(statistics->getAverageDecompositionTime(level)),
                                 tr("Composition time"), QString::number(statistics->getAverageCompositionTime(level)),
                                 tr("Outer routing time"), QString::number(statistics->getAverageOuterRoutingTime(level)))
                              .arg(tr("Total"), QString::number(statistics->getAverageTotalOuterTime(level)),
                                 tr("Inner routed wires percent"), QString::number(statistics->getAverageInnerRoutedWiresPercent(level), 'g', 3),
                                 tr("Routed wires percent"), QString::number(statistics->getAverageRoutedWiresPercent(level), 'g', 3)));
}

void StatisticsDialog::showHostData(int level, int index)
{
    StatisticsEntry entry = statistics->getData()[level][index];

    ui->textBrowser->append(QString("<h3 style=\"color: #33ccff;\">%1 %2</h3>"
                                    "<span style=\"font-size: 10pt;\">"
                                    "%3: %4ms<br>"
                                    "%5: %6ms<br>"
                                    "%7: %8ms<br>"
                                    "%9: %10ms<br>"
                                    "<br>"
                                    "%11: %12ms<br>"
                                    "%13: %14ms<br>"
                                    "%15: %16ms<br>"
                                    "%17: %18ms<br>"
                                    "<br>"
                                    "%19: %20ms<br>"
                                    "%21: %22ms<br>"
                                    "%23: %24%<br>"
                                    "%25: %26%<br>"
                                    "%27: %28%<br>""</span>")
                            .arg(tr("Computer"), QString::number(index + 1),
                                 tr("Primary placement time"), QString::number(entry.getPrimaryPlacememtTime()),
                                 tr("Secondary placement time"), QString::number(entry.getSecondaryPlacementTime()),
                                 tr("Inner routing time"), QString::number(entry.getInnerRoutingTime()))
                             .arg(tr("Total"), QString::number(entry.getTotalInnerTime()),
                                 tr("Decomposition time"), QString::number(entry.getDecompositionTime()),
                                 tr("Composition time"), QString::number(entry.getCompositionTime()),
                                 tr("Outer routing time"), QString::number(entry.getOuterRoutingTime()))
                              .arg(tr("Total"), QString::number(entry.getTotalOuterTime()),
                                 tr("Inner wires number"), QString::number(entry.getInnerWiresNumber()),
                                 tr("Wires number"), QString::number(entry.getWiresNumber()),
                                 tr("Outer wires percent"), QString::number(entry.getOuterWiresPercent()))
                              .arg(tr("Inner routed wires percent"), QString::number(entry.getInnerRoutedWiresPercent(), 'g', 3),
                                tr("Routed wires percent"), QString::number(entry.getRoutedWiresPercent(), 'g', 3)));

}
