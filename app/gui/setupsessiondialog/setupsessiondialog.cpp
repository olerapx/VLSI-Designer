#include "setupsessiondialog.h"
#include "ui_setupsessiondialog.h"

SetupSessionDialog::SetupSessionDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SetupSessionDialog),
    scheme(nullptr),
    architecture(nullptr)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    fillAlgorithms();
    fillSplittingTypes();
}

SetupSessionDialog::~SetupSessionDialog()
{
    delete ui;
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
    ui->splittingTypeBox->addItem(tr("Default"));
    ui->splittingTypeBox->addItem(tr("Greedy"));

    ui->splittingTypeBox->setCurrentIndex(0);
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
        searchLibraries();
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
}

void SetupSessionDialog::searchLibraries()
{

}
