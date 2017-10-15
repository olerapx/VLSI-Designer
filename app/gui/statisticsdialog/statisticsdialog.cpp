#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

StatisticsDialog::StatisticsDialog(Statistics* statistics, Grid* grid, FileSystem& system, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog),
    statistics(statistics),
    grid(grid),
    fileSystem(system)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    display();
}

StatisticsDialog::~StatisticsDialog()
{
    delete ui;
}

void StatisticsDialog::display()
{
    showHeader();
    showCommon();

    for(int i=0; i<statistics->getData().size(); i++)
        showDataForLevel(i);
}

void StatisticsDialog::showCommon()
{
    ui->textBrowser->append(QString("<hr><br><h2 style=\"color: #191970; text-align: center; text-transform: uppercase;\">%1</h2><br>"
                                    "<table align=\"center\" cellpadding=\"5\" style=\"font-size: 10pt;\">"
                                    "<tr><td>%2</td> <td>%3</td></tr>"
                                    "<tr><td>%4</td> <td>%5 (%6 ms)</td></tr>"
                                    "<tr><td>%7</td> <td>%8%</td></tr>"
                                    "<tr><td>%9</td> <td>%10</td></tr>"
                                    "<tr><td>%11</td> <td>%12</td></tr>"
                                    "<tr><td>%13</td> <td>%14</td></tr>")
                                .arg(tr("Common data"),
                                     tr("Used model"), getModel(),
                                     tr("Total time"), getTotalTimeString(), QString::number(getTotalTime()),
                                     tr("Routed wires percent"), QString::number(statistics->getAverageInnerRoutedWiresPercent(0), 'g', 3))
                                .arg(tr("Chip width"), QString::number(grid->getWidth()),
                                     tr("Chip height"), QString::number(grid->getHeight()),
                                     tr("Chip square"), QString::number(grid->getWidth() * grid->getHeight())));
}

int StatisticsDialog::getTotalTime()
{
    int res = 0;

    for(int i=0; i<statistics->getData().size(); i++)
    {
        res += statistics->getAverageTotalInnerTime(i);
        res += statistics->getAverageTotalOuterTime(i);
    }

    return res;
}

QString StatisticsDialog::getTotalTimeString()
{
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(getTotalTime());

    QString timeString = time.toString("hh:mm:ss.zzz");
    return timeString;
}

QString StatisticsDialog::getModel()
{
    QString res;

    for(int i=0; i<statistics->getData().size() - 1; i++)
    {
        int clientsNumber = statistics->getData()[i+1].size() / statistics->getData()[i].size();
        res += QString(" %1").arg(QString::number(clientsNumber));
    }

    return res;
}

void StatisticsDialog::showHeader()
{
    ui->textBrowser->append(QString("<h1 style=\"text-align: center; text-transform: uppercase;\">%1</h1>"
                                    "<p style=\"text-align: center; color: #191970; font-weight: bold\">%2</p>")
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
    QString text = QString("<hr><br><h2 style=\"color: #191970; text-align: center; text-transform: uppercase;\">%1 %2</h2>")
            .arg(tr("Level"), QString::number(level));

    if(level != 0)
            text.append(QString("<h3 style=\"color: #191970;\">%1</h3>"
                                    "<table align=\"center\" cellpadding=\"5\" style=\"font-size: 10pt;\">"
                                    "<tr><td>%2</td> <td>%3 ms</td></tr>"
                                    "<tr><td>%4</td> <td>%5 ms</td></tr>"
                                    "<tr><td>%6</td> <td>%7 ms</td></tr>"
                                    "<tr><td>%8</td> <td>%9 ms</td></tr>"
                                    "<tr></tr>"
                                    "<tr><td>%10</td> <td>%11 ms</td></tr>"
                                    "<tr><td>%12</td> <td>%13 ms</td></tr>"
                                    "<tr><td>%14</td> <td>%15 ms</td></tr>"
                                    "<tr><td>%16</td> <td>%17 ms</td></tr>"
                                    "<tr></tr>"
                                    "<tr><td>%18</td> <td>%19%</td></tr>"
                                    "<tr><td>%20</td> <td>%21%</td></tr></table><br>")
                            .arg(tr("Average"),
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
    ui->textBrowser->append(text);
}

void StatisticsDialog::showHostData(int level, int index)
{
    StatisticsEntry entry = statistics->getData()[level][index];

    ui->textBrowser->append(QString("<br><h3 style=\"color: #191970;\">%1 %2: %3</h3>"
                                    "<table align=\"center\" cellpadding=\"5\" style=\"font-size: 10pt;\">"
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
                                 tr("External wires percent"), QString::number(entry.getOuterWiresPercent(), 'g', 3))
                              .arg(tr("Internal routed wires percent"), QString::number(entry.getInnerRoutedWiresPercent(), 'g', 3),
                                tr("Routed wires percent"), QString::number(entry.getRoutedWiresPercent(), 'g', 3)));

}
