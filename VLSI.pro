TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += src app

CONFIG(debug, debug|release) {
    SUBDIRS += tests
    tests.depends = src
}
