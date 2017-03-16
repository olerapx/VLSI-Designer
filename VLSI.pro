TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += src app

CONFIG(debug, debug|release) {
    SUBDIRS += tests
    tests.depends = src
}

copydata.commands = $(COPY_DIR) $$shell_path($$PWD/libraries) $$shell_path($$OUT_PWD/libraries)
first.depends = $(first) copydata

export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata
