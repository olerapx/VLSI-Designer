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
    scrollTop();
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

void StatisticsDialog::showHeader()
{
    ui->textBrowser->append(QString("<h1 style=\"text-align: center; text-transform: uppercase;\">%1</h1>"
                                    "<p style=\"text-align: center; color: #191970; font-weight: bold\">%2</p>")
                                    .arg(tr("Session report"), fileSystem.getSessionName()));
}

void StatisticsDialog::showCommon()
{
    QString text = QString("<hr><br><h2 style=\"color: #191970; text-align: center; text-transform: uppercase;\">%1</h2><br>"
                            "<table align=\"center\" cellpadding=\"5\" style=\"font-size: 10pt;\">")
                            .arg(tr("Common data"));

    appendRow(text, tr("Used model"), getModel());
    appendRow(text, tr("Total time"), QString("%1 (%2 ms)").arg(getTotalTimeString(), QString::number(getTotalTime())));
    appendRow(text, tr("Routed wires number"), QString::number(statistics->getTotalRoutedWiresDelta()));
    appendRow(text, tr("Chip width"), QString::number(grid->getWidth()));
    appendRow(text, tr("Chip height"), QString::number(grid->getHeight()));
    appendRow(text, tr("Chip square"), QString::number(grid->getWidth() * grid->getHeight()));

    ui->textBrowser->append(text);
}

void StatisticsDialog::appendRow(QString& data, QString key, QString value)
{
    QString row = QString("<tr><td>%1</td> <td>%2</td></tr>").arg(key, value);
    data.append(row);
}

void StatisticsDialog::appendEmptyRow(QString& data)
{
    QString row("<tr></tr>");
    data.append(row);
}

int StatisticsDialog::getTotalTime()
{
    int res = 0;

    for(int i=0; i<statistics->getData().size(); i++)
    {
        res += statistics->getMaxTotalInternalTime(i);
        res += statistics->getMaxTotalExternalTime(i);
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
    {
        text.append(QString("<h3 style=\"color: #191970;\">%1</h3><table align=\"center\" cellpadding=\"5\" style=\"font-size: 10pt;\">")
                    .arg(tr("Average")));

        appendRow(text, tr("Primary placement time"), QString::number(statistics->getAveragePrimaryPlacementTime(level)));
        appendRow(text, tr("Secondary placement time"), QString::number(statistics->getAverageSecondaryPlacementTime(level)));
        appendRow(text, tr("Internal routing time"), QString::number(statistics->getAverageInternalRoutingTime(level)));
        appendRow(text, tr("Total"), QString::number(statistics->getAverageTotalInternalTime(level)));
        appendEmptyRow(text);

        appendRow(text, tr("Decomposition time"), QString::number(statistics->getAverageDecompositionTime(level)));
        appendRow(text, tr("Composition time"), QString::number(statistics->getAverageCompositionTime(level)));
        appendRow(text, tr("External routing time"), QString::number(statistics->getAverageExternalRoutingTime(level)));
        appendRow(text, tr("Total"), QString::number(statistics->getAverageTotalExternalTime(level)));
        appendEmptyRow(text);

        appendRow(text, tr("Routed wires number change"), QString::number(statistics->getAverageRoutedWiresDelta(level), 'g', 3));
        appendRow(text, tr("Internal routed wires percent"), QString::number(statistics->getAverageRoutedInternalWiresPercent(level), 'g', 3));
        appendRow(text, tr("Total routed wires percent"), QString::number(statistics->getAverageRoutedTotalWiresPercent(level), 'g', 3));
    }

    ui->textBrowser->append(text);
}

void StatisticsDialog::showHostData(int level, int index)
{
    StatisticsEntry entry = statistics->getData()[level][index];

    QString text = QString("<br><h3 style=\"color: #191970;\">%1 %2: %3</h3>"
                           "<table align=\"center\" cellpadding=\"5\" style=\"font-size: 10pt;\">")
                   .arg(tr("Computer"), QString::number(index + 1), entry.getHostName());

    appendRow(text, tr("Primary placement time"), QString::number(entry.getPrimaryPlacememtTime()));
    appendRow(text, tr("Secondary placement time"), QString::number(entry.getSecondaryPlacementTime()));
    appendRow(text, tr("Internal routing time"), QString::number(entry.getInternalRoutingTime()));
    appendRow(text, tr("Total"), QString::number(entry.getTotalInternalTime()));
    appendEmptyRow(text);

    appendRow(text, tr("Decomposition time"), QString::number(entry.getDecompositionTime()));
    appendRow(text, tr("Composition time"), QString::number(entry.getCompositionTime()));
    appendRow(text, tr("External routing time"), QString::number(entry.getExternalRoutingTime()));
    appendRow(text, tr("Total"), QString::number(entry.getTotalExternalTime()));
    appendEmptyRow(text);

    appendRow(text, tr("Internal unrouted wires number"), QString::number(entry.getInternalUnroutedWiresNumber()));
    appendRow(text, tr("Total unrouted wires number"), QString::number(entry.getTotalUnroutedWiresNumber()));
    appendEmptyRow(text);

    appendRow(text, tr("Routed wires number change"), QString::number(entry.getRoutedWiresDelta()));
    appendRow(text, tr("Internal routed wires percent"), QString::number(entry.getRoutedInternalWiresPercent(), 'g', 3));
    appendRow(text, tr("Total routed wires percent"), QString::number(entry.getRoutedTotalWiresPercent(), 'g', 3));

    ui->textBrowser->append(text);
}

void StatisticsDialog::scrollTop()
{
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);
}

void StatisticsDialog::on_openGridImageButton_clicked()
{
    QString imagePath = fileSystem.getLevelPath(0) + "/grid.png";
    QDesktopServices::openUrl(QUrl::fromLocalFile(imagePath));
}

void StatisticsDialog::on_openSessionDirectoryButton_clicked()
{
    QString sessionPath = fileSystem.getSessionPath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(sessionPath));
}
