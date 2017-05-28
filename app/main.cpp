#include <QApplication>
#include <QTranslator>

#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator appTranslator, srcTranslator;
    appTranslator.load(":/resources/localization/app_" + QLocale::system().name());
    srcTranslator.load(":/resources/localization/src_" + QLocale::system().name());

    a.installTranslator(&appTranslator);
    a.installTranslator(&srcTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
