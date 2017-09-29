TEMPLATE = lib
CONFIG += qt plugin
TARGET = csdataengineca
DESTDIR = $$top_builddir/plugins/csdataengine

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

LIBS += -L$$top_builddir/lib -lcsdata

SOURCES += \
    $$PWD/csdataengineca.cpp

HEADERS += \
    $$PWD/csdataengineca.h

include(../../../epics.pri)
include(../../../deployment.pri)
