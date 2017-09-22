#include "setupsessiondialog.h"
#include "ui_setupsessiondialog.h"

SetupSessionDialog::SetupSessionDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SetupSessionDialog)
{
    ui->setupUi(this);
}

SetupSessionDialog::~SetupSessionDialog()
{
    delete ui;
}
