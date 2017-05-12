#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

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

private slots:
    void on_librariesButton_clicked();

    void on_generateButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::GeneratorWindow *ui;

    QStringList libraryFiles;
    Generator* generator;

    void setValidators();

    GeneratorParameters buildParameters();

    QString saveScheme(Scheme s);
};

#endif // GENERATORWINDOW_H
