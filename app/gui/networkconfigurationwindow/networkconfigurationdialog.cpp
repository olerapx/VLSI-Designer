#include "networkconfigurationdialog.h"
#include "ui_networkconfigurationwindow.h"

NetworkConfigurationDialog::NetworkConfigurationDialog(Config& config, PoolManager& manager, bool firstTime, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NetworkConfigurationWindow),
    config(config),
    manager(manager),
    firstTime(firstTime)
{
    ui->setupUi(this);
    this->setFixedSize(this->sizeHint());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setValidators();

    fillNetworkInterfaces();
    readConfig();
}

void NetworkConfigurationDialog::setValidators()
{
    QIntValidator* intValidator = new QIntValidator(this);
    intValidator->setBottom(0);
    intValidator->setTop(65535);

    ui->udpPortText->setValidator(intValidator);
    ui->managerTcpPortText->setValidator(intValidator);
    ui->nodeTcpPortText->setValidator(intValidator);
}

void NetworkConfigurationDialog::fillNetworkInterfaces()
{
    interfaces = QNetworkInterface::allInterfaces();

    for(QNetworkInterface& i: interfaces)
        ui->networkInterfaceBox->addItem(i.humanReadableName());
}

void NetworkConfigurationDialog::readConfig()
{
    if(!firstTime)
    {
        int index = config.getNetworkInterface().index();

        for(int i=0; i<interfaces.size(); i++)
        {
            if(interfaces[i].index() == index)
                ui->networkInterfaceBox->setCurrentIndex(i);
        }
    }

    if(config.getMode() == Mode::IPv6)
    {
        ui->ipv4Radio->setChecked(false);
        ui->ipv6AddressText->setEnabled(true);
    }
    else
    {
        ui->ipv4Radio->setChecked(true);
        ui->ipv6AddressText->setEnabled(false);
    }

    ui->ipv6AddressText->setText(config.getMulticastAddress().toString());

    ui->udpPortText->setText(QString::number(config.getUdpPort()));
    ui->managerTcpPortText->setText(QString::number(config.getManagerTcpPort()));
    ui->nodeTcpPortText->setText(QString::number(config.getNodeTcpPort()));
}

void NetworkConfigurationDialog::closeEvent(QCloseEvent* event)
{
    try
    {
        if(firstTime)
            writeConfig();

        QDialog::closeEvent(event);
    }
    catch(Exception& e)
    {
        QMessageBox::critical(this, tr("Error"), e.what());
        event->ignore();
    }
}

NetworkConfigurationDialog::~NetworkConfigurationDialog()
{
    delete ui;
}

void NetworkConfigurationDialog::writeConfig()
{
    if(ui->ipv4Radio->isChecked())
        config.setMode(Mode::IPv4);
    else
        config.setMode(Mode::IPv6);

    config.setUdpPort(ui->udpPortText->text().toInt());
    config.setTcpPorts(ui->managerTcpPortText->text().toInt(), ui->nodeTcpPortText->text().toInt());

    config.setMulticastAddress(QHostAddress(ui->ipv6AddressText->text()));
    config.setNetworkInterface(interfaces[ui->networkInterfaceBox->currentIndex()]);
}

void NetworkConfigurationDialog::on_okButton_clicked()
{
    try
    {
        writeConfig();
        this->accept();
    }
    catch(Exception& e)
    {
        QMessageBox::critical(this, tr("Error"), e.what());
    }
}

void NetworkConfigurationDialog::on_ipv4Radio_toggled(bool checked)
{
    if(checked)
        ui->ipv6AddressText->setEnabled(false);
    else
        ui->ipv6AddressText->setEnabled(true);
}
