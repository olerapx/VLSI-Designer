#pragma once

#include <QApplication>
#include <QStyleFactory>
#include <QTranslator>
#include <QHostAddress>

#include "gui/mainwindow/mainwindow.h"
#include "datamodels/version/version.h"
#include "control/pool/entitytype.h"

/**
 * @brief The Loader class
 * Encapsulates the routines on application loading.
 */
class Loader
{
public:
    Loader(int argc, char* argv[]);
    int load();

private:
    void installTranslators();
    void setStyle();
    void registerMetaTypes();

    QApplication a;
};
