#pragma once

#include <QDialog>

namespace Ui
{
    class SetupSessionDialog;
}

class SetupSessionDialog : public QDialog
{
    Q_OBJECT

public:
    SetupSessionDialog(QWidget* parent = 0);
    ~SetupSessionDialog();

private:
    Ui::SetupSessionDialog* ui;
};
