QT += testlib
QT += core gui
CONFIG += qt warn_on depend_includepath testcase

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include (../../../../src/src.pri)
LIBS += -L../../../../src -lvlsi

TEMPLATE = app

SOURCES += tst_gridrenderertest.cpp

images.commands = $(COPY_DIR) $$shell_path($$PWD/images) $$shell_path($$OUT_PWD/images)
first.depends = $(first) images

export(first.depends)
export(images.commands)

QMAKE_EXTRA_TARGETS += first images
