#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeneratorWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->sizeHint());

    setValidators();
}

GeneratorWindow::~GeneratorWindow()
{
    delete ui;
}

void GeneratorWindow::setValidators()
{
    QIntValidator* intValidator = new QIntValidator(this);
    intValidator->setBottom(0);

    ui->elementsNumberText->setValidator(intValidator);

    ui->nodeCapacityMeanText->setValidator(intValidator);
    ui->nodeCapacityLeftLimitText->setValidator(intValidator);
    ui->nodeCapacityRightLimitText->setValidator(intValidator);

    ui->branchingMeanText->setValidator(intValidator);
    ui->branchingLeftLimitText->setValidator(intValidator);
    ui->branchingRightLimitText->setValidator(intValidator);

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);
    QLocale loc = QLocale::c();
    loc.setNumberOptions(QLocale::RejectGroupSeparator | QLocale::OmitGroupSeparator);
    doubleValidator->setLocale(loc);
    doubleValidator->setBottom(0.0);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    ui->nodeCapacitySigmaText->setValidator(doubleValidator);
    ui->branchingSigmaText->setValidator(doubleValidator);
}

void GeneratorWindow::on_librariesButton_clicked()
{
    QDir dir = QDir::currentPath();
    dir.cdUp();
    libraryFiles = QFileDialog::getOpenFileNames(this, "Выбор библиотек", dir.absolutePath(), "JSON (*.json);;Все файлы (*.*)");

    QString files = "";

    for(QString str: libraryFiles)
    {
        QFileInfo info(str);
        files.append(info.fileName() + " ");
    }

    ui->librariesText->setText(files);
}

void GeneratorWindow::on_generateButton_clicked()
{
    try
    {
        GeneratorParameters param = buildParameters();

        generator = new Generator(param); //TODO: threading
        Scheme s = generator->generate();

        delete generator;

        saveScheme(s);
    }
    catch(Exception ex)
    {
        QMessageBox::critical(this, "Ошибка", ex.what());
    }
}

GeneratorParameters GeneratorWindow::buildParameters()
{
    QList<Library*> libraries;
    JsonSerializer json;

    for(QString file: libraryFiles)
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        Library* l = static_cast<Library*>(json.deserialize(f.readAll()));
        f.close();

        libraries.append(l);
    }

    GeneratorParameters param(libraries);
    param.setElementsNumber(ui->elementsNumberText->text().toInt());

    param.setNodeCapacity(ui->nodeCapacityMeanText->text().toInt(),
                          ui->nodeCapacitySigmaText->text().toDouble(),
                          ui->nodeCapacityLeftLimitText->text().toInt(),
                          ui->nodeCapacityRightLimitText->text().toInt());

    param.setBranching(ui->branchingMeanText->text().toInt(),
                       ui->branchingSigmaText->text().toDouble(),
                       ui->branchingLeftLimitText->text().toInt(),
                       ui->branchingRightLimitText->text().toInt());

    return param;
}

void GeneratorWindow::saveScheme(Scheme s)
{
    JsonSerializer json;
    QByteArray array = json.serialize(&s);

    QDir dir = QDir::currentPath();
    dir.cdUp();

    QString file = QFileDialog::getSaveFileName(this, "Сохранение схемы", dir.absolutePath(), "JSON (*.json)");
    if(file.isEmpty())
        return;

    QFile f(file);
    f.open(QIODevice::WriteOnly);

    f.write(array);

    f.close();
}

void GeneratorWindow::on_closeButton_clicked()
{
    this->close();
}
