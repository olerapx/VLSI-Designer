#include "loader.h"

Loader::Loader(int argc, char* argv[]) :
    a(argc, argv)
{

}

int Loader::load()
{
    installTranslators();
    setStyle();
    registerMetaTypes();

    a.setWindowIcon(QIcon(":/resources/images/icons/app.ico"));

    MainWindow w;
    w.show();

    return a.exec();
}

void Loader::installTranslators()
{
    QTranslator* appTranslator = new QTranslator(&a);
    QTranslator* srcTranslator = new QTranslator(&a);

    appTranslator->load(":/resources/localization/app_" + QLocale::system().name());
    srcTranslator->load(":/resources/localization/src_" + QLocale::system().name());

    a.installTranslator(appTranslator);
    a.installTranslator(srcTranslator);
}

void Loader::setStyle()
{
#ifdef Q_OS_WIN
    a.setStyle(QStyleFactory::create("Fusion"));
#endif
}

void Loader::registerMetaTypes()
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<Version>("Version");
    qRegisterMetaType<EntityType>("EntityType");
    qRegisterMetaType<QList<Library*>>("QList<Library*>");
    qRegisterMetaType<QList<Scheme*>>("QList<Scheme*>");
    qRegisterMetaType<LogType>("LogType");
}
