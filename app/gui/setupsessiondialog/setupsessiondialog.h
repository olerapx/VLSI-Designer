#pragma once

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "datamodels/architecture/architecture.h"
#include "algorithms/composition/compositionstrategy.h"
#include "algorithms/decomposition/decompositionstrategy.h"
#include "algorithms/placement/primaryplacementstrategy.h"
#include "algorithms/placement/secondaryplacementstrategy.h"
#include "algorithms/routing/routingstrategy.h"
#include "util/serializer/serializerstrategy.h"

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

private slots:
    void on_schemeButton_clicked();

private:
    void fillAlgorithms();
    void fillSplittingTypes();

    void clear();

    void searchLibraries();

    Ui::SetupSessionDialog* ui;

    Scheme* scheme;
    QList<Library*> libraries;
    Architecture* architecture;

    QList<QPair<QString, double>> usedLibraries;
};
