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
    gui/networkconfigurationwindow/networkconfigurationdialog.h

SOURCES +=  main.cpp \
    gui/mainwindow/mainwindow.cpp \
    gui/generatorwindow/generatorwindow.cpp \
    viewmodels/nodeviewmodel.cpp \
    gui/addnodesdialog/addnodesdialog.cpp \
    viewmodels/addnodeviewmodel.cpp \
    config/config.cpp \
    gui/networkconfigurationwindow/networkconfigurationdialog.cpp

FORMS += \
    gui/mainwindow/mainwindow.ui \
    gui/generatorwindow/generatorwindow.ui \
    gui/addnodesdialog/addnodesdialog.ui \
    gui/networkconfigurationwindow/networkconfigurationwindow.ui

TRANSLATIONS = \
    ../resources/localization/app_en_US.ts  \
    ../resources/localization/app_ru_RU.ts
