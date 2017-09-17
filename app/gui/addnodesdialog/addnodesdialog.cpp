#include "addnodesdialog.h"
#include "ui_addnodesdialog.h"

AddNodesDialog::AddNodesDialog(PoolManager& manager, NetworkScanner& scanner, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddNodesDialog),
    manager(manager),
    scanner(scanner)
{
    ui->setupUi(this);

    addNodeViewModel = new AddNodeViewModel(this, foundNodes, manager.getPort());
    ui->nodesTable->setModel(addNodeViewModel);

    connect(&scanner, &NetworkScanner::sendLog, this, &AddNodesDialog::onSendLog);
    connect(&scanner, &NetworkScanner::sendAddress, this, &AddNodesDialog::onSendAddress);
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

    ui->scanButton->setText("Stop");

    addNodeViewModel->clear();

    scanner.scanNetwork();
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
