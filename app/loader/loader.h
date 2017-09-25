#pragma once

#include <QApplication>
#include <QStyleFactory>
#include <QTranslator>
#include <QHostAddress>

#include "gui/mainwindow/mainwindow.h"

Q_DECLARE_METATYPE(QHostAddress)

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
