#pragma once

#include <QDialog>
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>

#include "config/config.h"
#include "control/pool/manager/poolmanager.h"
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
    SetupSessionDialog(Config& config, PoolManager& manager, QWidget* parent = 0);
    ~SetupSessionDialog();

    Scheme* getScheme() { return scheme; }
    Architecture* getArchitecture() { return architecture; }
    QList<Library*>& getLibraries() { return libraries; }

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_schemeButton_clicked();

    void on_librariesButton_clicked();

    void on_okButton_clicked();

private:
    void setValidators();

    void initParameters();
    void fillAlgorithms();
    void fillSplittingTypes();

    void fillParametersFromData(SessionData* data);

    void clear();

    void searchLibraries();
    void tryOpenLibrary(QString path);
    void messageMissingLibraries();

    void writeParameters();

    Ui::SetupSessionDialog* ui;

    Config& config;
    PoolManager& manager;

    Scheme* scheme;
    QList<Library*> libraries;
    Architecture* architecture;

    QList<QPair<QString, Version>> usedLibraries;
};
