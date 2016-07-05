include(../../paths.pri)

TEMPLATE = lib
CONFIG += qt plugin
QT += quick
TARGET = csdataplugin
TARGETPATH = $$OUT_PWD/../../qml/CSDataQuick/Data
DESTDIR = $${TARGETPATH}

osx:QMAKE_RPATHDIR = @executable_path
else:unix:QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/../../../lib\'' 

INCLUDEPATH += $$PWD/../csdata/core
DEPENDPATH += $$PWD/../csdata/core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib -lcsdata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib -lcsdata
else:unix: LIBS += -L$$OUT_PWD/../../lib -lcsdata

SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/sortfilterproxymodel.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/sortfilterproxymodel.h

QML_FILES = \
    $$PWD/qmldir \
    $$PWD/plugins.qmltypes

include(../../rpath.pri)
include(../../qmlmodule.pri)
