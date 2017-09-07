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
    gui/mainwindow.h \
    gui/generatorwindow.h

SOURCES +=  main.cpp \
    gui/mainwindow.cpp \
    gui/generatorwindow.cpp

FORMS += \
    gui/mainwindow.ui \
    gui/generatorwindow.ui

TRANSLATIONS = \
    ../resources/localization/app_en_US.ts  \
    ../resources/localization/app_ru_RU.ts
