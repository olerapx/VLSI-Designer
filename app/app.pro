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

include (../src/src.pri)
LIBS += -L../src -lvlsi

HEADERS += \
    gui/mainwindow.h \

SOURCES +=  main.cpp \
    gui/mainwindow.cpp \

FORMS    += gui/mainwindow.ui
