TEMPLATE = subdirs

SUBDIRS += algorithms util generator datamodels

win32: copydata.commands = $(COPY_DIR) $$shell_path($$PWD/datamodels) $$shell_path($$OUT_PWD/datamodels)
!win32: copydata.commands = $(COPY_DIR) $$shell_path($$PWD/datamodels) $$shell_path($$OUT_PWD)

first.depends = $(first) copydata

export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata
