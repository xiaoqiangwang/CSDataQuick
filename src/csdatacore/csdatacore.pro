include(../../paths.pri)

TEMPLATE = lib
CONFIG += qt plugin
QT += quick
TARGET = csdataplugin
TARGETPATH = CSDataQuick/Data
DESTDIR = $$top_builddir/qml/$$TARGETPATH

INCLUDEPATH += $$PWD/../csdata/core
DEPENDPATH += $$PWD/../csdata/core

LIBS += -L$$top_builddir/lib -lcsdata

SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/sortfilterproxymodel.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/sortfilterproxymodel.h

QML_FILES = \
    $$PWD/qmldir \
    $$PWD/plugins.qmltypes

target.path = $$PREFIX/qml/$$TARGETPATH
INSTALLS += target

include(../../rpath.pri)
include(../../qmlmodule.pri)
