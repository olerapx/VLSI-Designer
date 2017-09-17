#include "addnodesdialog.h"
#include "ui_addnodesdialog.h"

AddNodesDialog::AddNodesDialog(PoolManager& manager, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddNodesDialog),
    manager(manager)
{
    ui->setupUi(this);

    addNodeViewModel = new AddNodeViewModel(this, foundNodes, manager.getPort());
    ui->nodesTable->setModel(addNodeViewModel);

    if(manager.getUsedInterface() == nullptr)
    {
        fillNetworkInterfaces();
    }
    else
    {
        ui->networkInterfaceBox->setEnabled(false);
        ui->networkInterfaceBox->addItem(manager.getUsedInterface()->humanReadableName());
        ui->networkInterfaceBox->setCurrentIndex(0);

        ui->ipv4Radio->setEnabled(false);
        ui->ipv6Radio->setEnabled(false);

        if(manager.getUsedMode() == Mode::IPv4)
            ui->ipv4Radio->setChecked(true);
        else
            ui->ipv6Radio->setChecked(true);

        interfaces.append(*manager.getUsedInterface());
    }

    connect(&scanner, &NetworkScanner::sendLog, this, &AddNodesDialog::onSendLog);
    connect(&scanner, &NetworkScanner::sendAddress, this, &AddNodesDialog::onSendAddress);
}

void AddNodesDialog::fillNetworkInterfaces()
{
    interfaces = QNetworkInterface::allInterfaces();

    for (QNetworkInterface& i: interfaces)
        ui->networkInterfaceBox->addItem(i.humanReadableName());
}

AddNodesDialog::~AddNodesDialog()
{
    disconnect(&scanner, &NetworkScanner::sendLog, this, &AddNodesDialog::onSendLog);
    disconnect(&scanner, &NetworkScanner::sendAddress, this, &AddNodesDialog::onSendAddress);

    delete ui;
}

void AddNodesDialog::on_scanButton_clicked()
{
    if (!scanner.isStopped())
    {
        scanner.stopScanning();
        ui->scanButton->setText("Scan");

        return;
    }
    //TODO: move to port from config
    if(ui->ipv4Radio->isChecked())
        scanner.initIPv4Broadcast(interfaces[ui->networkInterfaceBox->currentIndex()], 41000, 41001);
    else
        scanner.initIPv6Multicast(QHostAddress(ui->ipv6AddressText->text()), interfaces[ui->networkInterfaceBox->currentIndex()],
                41000, 41001);

    ui->scanButton->setText("Stop");

    addNodeViewModel->clear();

    scanner.scanNetwork();
}

void AddNodesDialog::on_ipv4Radio_toggled(bool checked)
{
    if(checked)
        ui->ipv6AddressText->setEnabled(false);
    else
        ui->ipv6AddressText->setEnabled(true);
}

void AddNodesDialog::onSendLog(QString data)
{
    //TODO
}

void AddNodesDialog::onSendAddress(QHostAddress senderHost, QString hostName)
{
    if(hasAddress(senderHost))
        return;

    addNodeViewModel->appendRow(PoolNodeInfo(hostName, senderHost));
}

bool AddNodesDialog::hasAddress(QHostAddress address)
{
    for(PoolNodeInfo& info: manager.getPoolNodes())
    {
        if(info.getAddress().isEqual(address))
            return true;
    }

    return false;
}
