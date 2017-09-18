#include "addnodesdialog.h"
#include "ui_addnodesdialog.h"

AddNodesDialog::AddNodesDialog(PoolManager& manager, NetworkScanner& scanner, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddNodesDialog),
    manager(manager),
    scanner(scanner)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    addNodeViewModel = new AddNodeViewModel(this, foundNodes, manager.getPort());
    ui->nodesTable->setModel(addNodeViewModel);

    connect(&scanner, &NetworkScanner::sendLog, this, &AddNodesDialog::onSendLog);
    connect(&scanner, &NetworkScanner::sendAddress, this, &AddNodesDialog::onSendAddress);

    on_scanButton_clicked();
}

AddNodesDialog::~AddNodesDialog()
{
    disconnect(&scanner, &NetworkScanner::sendLog, this, &AddNodesDialog::onSendLog);
    disconnect(&scanner, &NetworkScanner::sendAddress, this, &AddNodesDialog::onSendAddress);

    delete ui;
}

void AddNodesDialog::closeEvent(QCloseEvent* event)
{
    scanner.stopScanning();
    QDialog::closeEvent(event);
}

void AddNodesDialog::on_scanButton_clicked()
{
    if (!scanner.isStopped())
    {
        scanner.stopScanning();
        ui->scanButton->setText("Scan");

        ui->statusLabel->clear();

        return;
    }

    ui->scanButton->setText("Stop");

    addNodeViewModel->clear();    

    scanner.scanNetwork();
}

void AddNodesDialog::onSendLog(QString data)
{
    ui->statusLabel->setText(data);
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

void AddNodesDialog::on_addSelectedButton_clicked()
{
    QItemSelectionModel* model = ui->nodesTable->selectionModel();
    QModelIndexList list = model->selectedRows();

    for(QModelIndex& index: list)
    {
        int row = index.row();
        selectedNodes.append(foundNodes[row]);
    }

    this->accept();
}

void AddNodesDialog::on_addAllButton_clicked()
{
    selectedNodes = foundNodes;
    this->accept();
}
