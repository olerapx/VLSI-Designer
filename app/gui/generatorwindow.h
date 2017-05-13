#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QTime>

#include "generator/generator.h"
#include "util/jsonserializer.h"

namespace Ui {
class GeneratorWindow;
}

class GeneratorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GeneratorWindow(QWidget *parent = 0);
    ~GeneratorWindow();

protected:
     void closeEvent(QCloseEvent *event);

private slots:
    void on_librariesButton_clicked();

    void on_generateButton_clicked();

    void on_closeButton_clicked();

    void onSendScheme(Scheme* s);
    void onSendError(QString error);
    void onSendLog(QString log);
    void onSendFinish();

    void on_stopButton_clicked();

signals:
    void sendStop();

private:
    Ui::GeneratorWindow *ui;

    QStringList libraryFiles;
    Generator* generator;

    QThread generatorThread;

    void setValidators();

    GeneratorParameters buildParameters();

    void saveScheme(Scheme s);
};

#endif // GENERATORWINDOW_H
