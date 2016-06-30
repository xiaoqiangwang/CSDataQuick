include(../../epicsqml.pri)

TEMPLATE = lib
CONFIG += qt plugin
QT += quick
TARGET = csdataplugin
TARGETPATH = $$OUTPUT_PATH/imports/CSDataQuick/Data/
DESTDIR = $${TARGETPATH}

osx:QMAKE_RPATHDIR = @executable_path
else:unix:QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/../../../bin\'' 

INCLUDEPATH += $$PWD/../csdata/core
DEPENDPATH += $$PWD/../csdata/core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../bin/ -lcsdata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../bin -lcsdata
else:unix: LIBS += -L$$OUT_PWD/../../bin/ -lcsdata

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
