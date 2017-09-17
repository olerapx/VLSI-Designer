#include "networkconfigurationdialog.h"
#include "ui_networkconfigurationwindow.h"

NetworkConfigurationDialog::NetworkConfigurationDialog(Config& config, PoolManager& manager, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NetworkConfigurationWindow),
    config(config),
    manager(manager)
{
    ui->setupUi(this);

    fillNetworkInterfaces();
    readConfig();
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
    {
        ui->ipv6Radio->setEnabled(true);
        ui->ipv6AddressText->setEnabled(true);
    }
    else
    {
        ui->ipv6Radio->setEnabled(false);
        ui->ipv6AddressText->setEnabled(false);
    }

    ui->ipv6AddressText->setText(config.getMulticastAddress().toString());
    ui->portText->setText(QString::number(config.getPort()));
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

    config.setPort(ui->portText->text().toInt());

    config.setMulticastAddress(QHostAddress(ui->ipv6AddressText->text()));

    manager.setPort(ui->portText->text().toInt()); //TODO: is it a bad idea to set parameters in manager here?
    manager.setUsedInterface(interfaces[ui->networkInterfaceBox->currentIndex()]);
}

void NetworkConfigurationDialog::on_okButton_clicked()
{
    this->close();
}

void NetworkConfigurationDialog::on_ipv4Radio_toggled(bool checked)
{
    if(checked)
    {
        ui->ipv6Radio->setEnabled(false);
        ui->ipv6AddressText->setEnabled(false);
    }
    else
    {
        ui->ipv6Radio->setEnabled(true);
        ui->ipv6AddressText->setEnabled(true);
    }
}
