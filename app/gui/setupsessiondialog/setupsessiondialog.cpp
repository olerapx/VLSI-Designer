#include "setupsessiondialog.h"
#include "ui_setupsessiondialog.h"

SetupSessionDialog::SetupSessionDialog(Config& config, PoolManager& manager, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SetupSessionDialog),
    config(config),
    manager(manager),
    scheme(nullptr),
    architecture(nullptr)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setValidators();
    initParameters();
}

SetupSessionDialog::~SetupSessionDialog()
{
    delete ui;
}

void SetupSessionDialog::setValidators()
{
    QRegExpValidator* modelValidator = new QRegExpValidator(QRegExp("^([1-9]+,( )*)+$"), this);
    ui->distributionModelText->setValidator(modelValidator);

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);
    QLocale loc = QLocale::c();
    loc.setNumberOptions(QLocale::RejectGroupSeparator | QLocale::OmitGroupSeparator);
    doubleValidator->setLocale(loc);
    doubleValidator->setBottom(0.0);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    ui->expandingCoefficientText->setValidator(doubleValidator);

    QIntValidator* intValidator = new QIntValidator(this);
    intValidator->setBottom(0);
    ui->maxExtensionAttemptsText->setValidator(intValidator);
}

void SetupSessionDialog::initParameters()
{
    fillAlgorithms();
    fillSplittingTypes();

    SessionData* data = manager.getSessionData();

    if(!data->isEmpty())
       fillParametersFromData(data);
}

void SetupSessionDialog::fillAlgorithms()
{
    CompositionStrategy cf;
    for(QString& s: cf.getAlgorithms())
        ui->compositionBox->addItem(s);

    ui->compositionBox->setCurrentIndex(0);

    DecompositionStrategy df;
    for(QString& s: df.getAlgorithms())
        ui->decompositionBox->addItem(s);

    ui->decompositionBox->setCurrentIndex(0);

    PrimaryPlacementStrategy ppf;
    for(QString& s: ppf.getAlgorithms())
        ui->primaryPlacementBox->addItem(s);

    ui->primaryPlacementBox->setCurrentIndex(0);

    SecondaryPlacementStrategy spf;
    for(QString& s: spf.getAlgorithms())
        ui->secondaryPlacementBox->addItem(s);

    ui->secondaryPlacementBox->setCurrentIndex(0);

    RoutingStrategy rf;
    for(QString& s: rf.getAlgorithms())
        ui->routingBox->addItem(s);

    ui->routingBox->setCurrentIndex(0);
}

void SetupSessionDialog::fillSplittingTypes()
{
    ui->distributionTypeBox->addItem(tr("Default"));
    ui->distributionTypeBox->addItem(tr("Greedy"));

    ui->distributionTypeBox->setCurrentIndex(0);
}

void SetupSessionDialog::fillParametersFromData(SessionData* data)
{
    AlgorithmIndexes indexes = data->getArchitecture()->getAlgorithmIndexes();

    ui->compositionBox->setCurrentIndex(indexes.getCompositionAlgorithmIndex());
    ui->decompositionBox->setCurrentIndex(indexes.getDecompositionAlgorithmIndex());
    ui->primaryPlacementBox->setCurrentIndex(indexes.getPrimaryPlacementAlgorithmIndex());
    ui->secondaryPlacementBox->setCurrentIndex(indexes.getSecondaryPlacementAlgorithmIndex());
    ui->routingBox->setCurrentIndex(indexes.getRoutingAlgorithmIndex());

    AlgorithmParameters parameters = data->getArchitecture()->getAlgorithmParameters();
    ui->expandingCoefficientText->setText(QString::number(parameters.getExpandingCoefficient()));
    ui->maxExtensionAttemptsText->setText(QString::number(parameters.getMaxExtensionAttempts()));

    DistributionType type = data->getArchitecture()->getDistributionType();
    if(type == DistributionType::Default)
        ui->distributionTypeBox->setCurrentIndex(0);
    else
        ui->distributionTypeBox->setCurrentIndex(1);

    QString numbers;
    if(!data->getArchitecture()->getModel().isEmpty())
    {
        numbers = QString::number(data->getArchitecture()->getModel()[0]);
        for(int i=1; i<data->getArchitecture()->getModel().size(); i++)
            numbers.append(QString(", %1").arg(QString::number(data->getArchitecture()->getModel()[i])));
    }

    ui->distributionModelText->setText(numbers);
}

void SetupSessionDialog::closeEvent(QCloseEvent* event)
{
    if(result() != QDialog::Accepted)
        clear();

    QDialog::closeEvent(event);
}

void SetupSessionDialog::on_schemeButton_clicked()
{
    QDir dir = QDir::currentPath();

    QString schemeFileName = QFileDialog::getOpenFileName(this, tr("Scheme choosing"), dir.absolutePath(),
                                                          tr("All supported files (*.bin *.json);;JSON (*.json);;Binary (*.bin)"));

    if(schemeFileName.isEmpty())
        return;

    clear();

    QFile file(schemeFileName);
    Serializer* serializer = SerializerStrategy::createSerializer(file);

    file.open(QIODevice::ReadOnly);

    try
    {
        scheme = dynamic_cast<Scheme*>(serializer->deserialize(file.readAll()));

        if(scheme == nullptr)
            throw Exception(tr("The chosen file does not contain a scheme."));

        ui->schemeText->setText(QFileInfo(file).fileName());

        usedLibraries.append(scheme->getUsedLibraries());
    }
    catch(Exception& e)
    {
        QMessageBox::critical(this, tr("Error"), e.what());
    }

    file.close();

    delete serializer;

    if(!usedLibraries.empty())
    {
        ui->okButton->setEnabled(false);
        searchLibraries();
    }
}

void SetupSessionDialog::clear()
{
    if(scheme != nullptr)
    {
        delete scheme;
        scheme = nullptr;
    }

    for(Library* l: libraries)
        delete l;

    libraries.clear();

    if(architecture != nullptr)
    {
        delete architecture;
        architecture = nullptr;
    }

    usedLibraries.clear();

    ui->schemeText->clear();
    ui->librariesText->clear();

    ui->librariesButton->setEnabled(false);
    ui->librariesText->setEnabled(false);
}

void SetupSessionDialog::searchLibraries()
{
    QDirIterator it(config.getLibrariesPath(), QStringList(), QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDirIterator::NoIteratorFlags);

    while (it.hasNext())
    {
        QString file = it.next();
        tryOpenLibrary(file);
    }

    if(!usedLibraries.empty())
    {
        messageMissingLibraries();

        ui->librariesButton->setEnabled(true);
        ui->librariesText->setEnabled(true);
    }
}

void SetupSessionDialog::tryOpenLibrary(QString path)
{
    QFile file(path);

    Serializer* serializer = SerializerStrategy::createSerializer(file);

    file.open(QIODevice::ReadOnly);

    try
    {
        Library* l = dynamic_cast<Library*>(serializer->deserialize(file.readAll()));
        if(l == nullptr)
            throw Exception(tr("The chosen file does not contain a library."));

        QPair<QString, Version> libraryData(l->getId(), l->getVersion());

        if(usedLibraries.contains(libraryData))
        {
            usedLibraries.removeAll(libraryData);

            if(!libraries.contains(l))
                libraries.append(l);

            ui->librariesText->setText(ui->librariesText->text() + " " + QFileInfo(file).fileName());
        }

        if(usedLibraries.empty())
            ui->okButton->setEnabled(true);
    }
    catch(Exception&)
    {
    }

    file.close();

    delete serializer;
}

void SetupSessionDialog::messageMissingLibraries()
{
    QString missingLibraries = tr("The following libraries used to generate scheme were not found:\n\n");

    for(QPair<QString, Version>& u: usedLibraries)
        missingLibraries.append(QString("%1: %2\n").arg(u.first, u.second.toString()));

    missingLibraries.append(tr("\nYou can try to locate necessary libraries manually."));

    QMessageBox::information(this, tr("Missing libraries"), missingLibraries);
}

void SetupSessionDialog::on_librariesButton_clicked()
{
    QStringList libraryFiles = QFileDialog::getOpenFileNames(this, tr("Libraries choosing"), config.getLibrariesPath(),
                                                          tr("All supported files (*.bin *.json);;JSON (*.json);;Binary (*.bin)"));

    if(libraryFiles.empty())
    {
        messageMissingLibraries();
        return;
    }

    for(QString& f: libraryFiles)
        tryOpenLibrary(f);

    if(!usedLibraries.empty())
        messageMissingLibraries();
}

void SetupSessionDialog::on_okButton_clicked()
{
    try
    {
        writeParameters();
    }
    catch(Exception& e)
    {
        QMessageBox::critical(this, tr("Error"), e.what());
        return;
    }


    accept();
    this->close();
}

void SetupSessionDialog::writeParameters()
{
    DistributionType type = ((ui->distributionTypeBox->currentIndex() == 0) ? DistributionType::Default : DistributionType::Greedy);
    AlgorithmIndexes indexes;

    indexes.setCompositionAlgorithmIndex(ui->compositionBox->currentIndex());
    indexes.setDecompositionAlgorithmIndex(ui->decompositionBox->currentIndex());
    indexes.setPrimaryPlacementAlgorithmIndex(ui->primaryPlacementBox->currentIndex());
    indexes.setSecondaryPlacementAlgorithmIndex(ui->secondaryPlacementBox->currentIndex());
    indexes.setRoutingAlgorithmIndex(ui->routingBox->currentIndex());

    AlgorithmParameters parameters;
    parameters.setExpandingCoefficient(ui->expandingCoefficientText->text().toDouble());
    parameters.setMaxExtensionAttempts(ui->maxExtensionAttemptsText->text().toInt());

    architecture = new Architecture(type, indexes, parameters);

    QString clientsNumber = ui->distributionModelText->text();
    QRegExp rx("(\\,)");

    QStringList numbers = clientsNumber.split(rx, QString::SkipEmptyParts);

    for(QString& s: numbers)
        architecture->getModel().append(s.toInt());
}
