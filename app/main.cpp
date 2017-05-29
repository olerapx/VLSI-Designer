#include <QApplication>
#include <QStyleFactory>
#include <QTranslator>

#include "gui/mainwindow.h"

void setStyle(QApplication& a);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator appTranslator, srcTranslator;
    appTranslator.load(":/resources/localization/app_" + QLocale::system().name());
    srcTranslator.load(":/resources/localization/src_" + QLocale::system().name());

    a.installTranslator(&appTranslator);
    a.installTranslator(&srcTranslator);

    setStyle(a);

    MainWindow w;
    w.show();

    return a.exec();
}

void setStyle(QApplication& a)
{
#ifdef Q_OS_WIN
    a.setStyle(QStyleFactory::create("Fusion"));
#endif
}
