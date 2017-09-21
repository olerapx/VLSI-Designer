#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeneratorWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->sizeHint());
    generator = nullptr;

    setValidators();
}

GeneratorWindow::~GeneratorWindow()
{
    delete ui;
}

void GeneratorWindow::closeEvent(QCloseEvent* event)
{
    onSendLog(tr("Stopping..."));
    sendStop();

    generatorThread.quit();
    generatorThread.wait();

    QMainWindow::closeEvent(event);
}

void GeneratorWindow::onSendScheme(Scheme* s)
{
    saveScheme(*s);

    delete s;
}

void GeneratorWindow::onSendError(QString error)
{
    QMessageBox::critical(this, tr("Error"), error);
}

void GeneratorWindow::onSendLog(QString log)
{
    ui->textBrowser->append(QString("[%1] %2").arg(QTime::currentTime().toString(), log));
}

void GeneratorWindow::onFinish()
{
    disconnect(&generatorThread, &QThread::started, generator, &Generator::onStart);
    disconnect(this, &GeneratorWindow::sendStop, generator, &Generator::onStop);

    disconnect(generator, &Generator::sendResult, this, &GeneratorWindow::onSendScheme);
    disconnect(generator, &Generator::sendError, this, &GeneratorWindow::onSendError);
    disconnect(generator, &Generator::sendLog, this, &GeneratorWindow::onSendLog);
    disconnect(generator, &Generator::sendFinish, this, &GeneratorWindow::onFinish);
    disconnect(generator, &Generator::sendFinish, &generatorThread, &QThread::quit);

    delete generator;
    generator = nullptr;

    for(Library* l: libraries)
        delete l;

    libraries.clear();

    onSendLog(tr("Generation is finished."));

    ui->generateButton->setEnabled(true);
}

void GeneratorWindow::setValidators()
{
    QIntValidator* intValidator = new QIntValidator(this);
    intValidator->setBottom(0);

    ui->elementsNumberText->setValidator(intValidator);

    ui->nodeCapacityMeanText->setValidator(intValidator);
    ui->nodeCapacityLowerLimitText->setValidator(intValidator);
    ui->nodeCapacityUpperLimitText->setValidator(intValidator);

    ui->branchingMeanText->setValidator(intValidator);
    ui->branchingLowerLimitText->setValidator(intValidator);
    ui->branchingUpperLimitText->setValidator(intValidator);

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);
    QLocale loc = QLocale::c();
    loc.setNumberOptions(QLocale::RejectGroupSeparator | QLocale::OmitGroupSeparator);
    doubleValidator->setLocale(loc);
    doubleValidator->setBottom(0.0);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    ui->innerWireChanceText->setValidator(doubleValidator);
    ui->nodeCapacitySigmaText->setValidator(doubleValidator);
    ui->branchingSigmaText->setValidator(doubleValidator);
}

void GeneratorWindow::on_librariesButton_clicked()
{
    QDir dir = QDir::currentPath();
    libraryFiles = QFileDialog::getOpenFileNames(this, tr("Libraries choosing"), dir.absolutePath(), tr("All supported files (*.bin *.json);;JSON (*.json);;Binary (*.bin)"));

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
    ui->generateButton->setEnabled(false);
    ui->textBrowser->clear();

    try
    {
        GeneratorParameters param = buildParameters();

        generator = new Generator(param);

        connect(&generatorThread, &QThread::started, generator, &Generator::onStart);
        connect(this, &GeneratorWindow::sendStop, generator, &Generator::onStop, Qt::DirectConnection);

        connect(generator, &Generator::sendResult, this, &GeneratorWindow::onSendScheme);
        connect(generator, &Generator::sendError, this, &GeneratorWindow::onSendError);
        connect(generator, &Generator::sendLog, this, &GeneratorWindow::onSendLog);
        connect(generator, &Generator::sendFinish, this, &GeneratorWindow::onFinish);
        connect(generator, &Generator::sendFinish, &generatorThread, &QThread::quit);

        generator->moveToThread(&generatorThread);

        generatorThread.start();
    }
    catch(Exception& e)
    {
        onSendError(e.what());
        ui->generateButton->setEnabled(true);
    }
}

GeneratorParameters GeneratorWindow::buildParameters()
{
    JsonSerializer jsonSerializer;
    BinarySerializer binarySerializer;

    for(QString file: libraryFiles)
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);

        QFileInfo info(f);
        Library* l;

        if(info.suffix().toLower() == "bin")
            l = static_cast<Library*>(binarySerializer.deserialize(f.readAll()));
        else
            l = static_cast<Library*>(jsonSerializer.deserialize(f.readAll()));

        f.close();

        libraries.append(l);
    }

    GeneratorParameters param(libraries);
    param.setElementsNumber(ui->elementsNumberText->text().toInt());
    param.setInnerWireChance(ui->innerWireChanceText->text().toDouble());

    param.setNodeCapacity(ui->nodeCapacityMeanText->text().toInt(),
                          ui->nodeCapacitySigmaText->text().toDouble(),
                          ui->nodeCapacityLowerLimitText->text().toInt(),
                          ui->nodeCapacityUpperLimitText->text().toInt());

    param.setBranching(ui->branchingMeanText->text().toInt(),
                       ui->branchingSigmaText->text().toDouble(),
                       ui->branchingLowerLimitText->text().toInt(),
                       ui->branchingUpperLimitText->text().toInt());

    return param;
}

void GeneratorWindow::saveScheme(Scheme s)
{
    onSendLog(tr("Serialization..."));

    JsonSerializer jsonSerializer;
    BinarySerializer binarySerializer;
    QByteArray array;

    QDir dir = QDir::currentPath();
    dir.cdUp();

    QString file = QFileDialog::getSaveFileName(this, tr("Scheme saving"), dir.absolutePath(), tr("JSON (*.json);;Binary (*.bin)"));
    if(file.isEmpty())
        return;

    QFile f(file);
    f.open(QIODevice::WriteOnly);

    QFileInfo info(f);
    if(info.suffix().toLower() == "bin")
        array = binarySerializer.serialize(&s);
    else
        array = jsonSerializer.serialize(&s);

    f.write(array);

    f.close();
}

void GeneratorWindow::on_closeButton_clicked()
{
    this->close();
}

void GeneratorWindow::on_stopButton_clicked()
{
    onSendLog(tr("Stopping..."));
    sendStop();
}
