#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QMessageBox>

#include "network/networkscanner.h"

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

    void on_sendLog(QString data);
    void on_sendAddress(QHostAddress senderHost, QString hostName);

private:
    Ui::MainWindow *ui;

    QList<QNetworkInterface> interfaces;

    NetworkScanner scanner;

    void showNetworkInterfaces();

    void log(QString text);
};

#endif // MAINWINDOW_H
