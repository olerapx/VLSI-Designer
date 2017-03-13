
TEMPLATE = subdirs

SUBDIRS += util
copydata.commands = $(COPY_DIR) $$PWD/datamodels $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
