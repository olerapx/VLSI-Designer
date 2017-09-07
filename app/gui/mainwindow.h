#pragma once

#include <QMainWindow>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QMessageBox>

#include "generatorwindow.h"
#include "network/networkscanner.h"
#include "network/networktransmitter.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_scanButton_clicked();
    void on_sendButton_clicked();

    void on_saveButton_clicked();

    void on_generatorAction_triggered();

    void on_sendLog(QString data);
    void on_sendAddress(QHostAddress senderHost, QString hostName);

    void on_dataReceived(QByteArray data,QHostAddress address,int port);

private:
    Ui::MainWindow *ui;

    QList<QNetworkInterface> interfaces;

    NetworkScanner scanner;
    NetworkTransmitter *clientT, *serverT;

    void showNetworkInterfaces();

    void log(QString text);
};
