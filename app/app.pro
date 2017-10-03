#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T17:55:40
#
#-------------------------------------------------

TARGET = VLSI
TEMPLATE = app

QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD

include(../src/src.pri)
LIBS += -L../src -lvlsi

HEADERS += \
    gui/mainwindow/mainwindow.h \
    gui/generatorwindow/generatorwindow.h \
    viewmodels/nodeviewmodel.h \
    gui/addnodesdialog/addnodesdialog.h \
    viewmodels/addnodeviewmodel.h \
    config/config.h \
    gui/networkconfigurationwindow/networkconfigurationdialog.h \
    gui/setupsessiondialog/setupsessiondialog.h \
    config/configbuilder.h \
    loader/loader.h \
    app.h \
    viewmodels/entitystatusdelegate.h

SOURCES +=  main.cpp \
    gui/mainwindow/mainwindow.cpp \
    gui/generatorwindow/generatorwindow.cpp \
    viewmodels/nodeviewmodel.cpp \
    gui/addnodesdialog/addnodesdialog.cpp \
    viewmodels/addnodeviewmodel.cpp \
    config/config.cpp \
    gui/networkconfigurationwindow/networkconfigurationdialog.cpp \
    gui/setupsessiondialog/setupsessiondialog.cpp \
    config/configbuilder.cpp \
    loader/loader.cpp \
    app.cpp \
    viewmodels/entitystatusdelegate.cpp

FORMS += \
    gui/mainwindow/mainwindow.ui \
    gui/generatorwindow/generatorwindow.ui \
    gui/addnodesdialog/addnodesdialog.ui \
    gui/networkconfigurationwindow/networkconfigurationwindow.ui \
    gui/setupsessiondialog/setupsessiondialog.ui

TRANSLATIONS = \
    ../resources/localization/app_en_US.ts  \
    ../resources/localization/app_ru_RU.ts
