#include "networkconfigurationdialog.h"
#include "ui_networkconfigurationwindow.h"

NetworkConfigurationDialog::NetworkConfigurationDialog(Config& config, PoolManager& manager, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NetworkConfigurationWindow),
    config(config),
    manager(manager)
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
    ui->tcpPortText->setValidator(intValidator);
}

void NetworkConfigurationDialog::fillNetworkInterfaces()
{
    interfaces = QNetworkInterface::allInterfaces();

    for (QNetworkInterface& i: interfaces)
        ui->networkInterfaceBox->addItem(i.humanReadableName());
}

void NetworkConfigurationDialog::readConfig()
{
    if(config.getMode() == Mode::IPv6)
        ui->ipv6AddressText->setEnabled(true);
    else
        ui->ipv6AddressText->setEnabled(false);

    ui->ipv6AddressText->setText(config.getMulticastAddress().toString());

    ui->udpPortText->setText(QString::number(config.getUdpPort()));
    ui->tcpPortText->setText(QString::number(config.getTcpPort()));
}

void NetworkConfigurationDialog::closeEvent(QCloseEvent* event)
{
    if(!config.isInterfaceSet())
        writeConfig();

    QDialog::closeEvent(event);
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
    config.setTcpPort(ui->tcpPortText->text().toInt());

    config.setMulticastAddress(QHostAddress(ui->ipv6AddressText->text()));

    manager.setPort(ui->tcpPortText->text().toInt());
    config.setNetworkInterface(interfaces[ui->networkInterfaceBox->currentIndex()]);
}

void NetworkConfigurationDialog::on_okButton_clicked()
{
    writeConfig();
    this->close();
}

void NetworkConfigurationDialog::on_ipv4Radio_toggled(bool checked)
{
    if(checked)
        ui->ipv6AddressText->setEnabled(false);
    else
        ui->ipv6AddressText->setEnabled(true);
}
