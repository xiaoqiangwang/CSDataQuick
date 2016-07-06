TEMPLATE = lib
CONFIG += qt plugin
TARGET = csdataengineca
DESTDIR = $$OUT_PWD/../../../plugins

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../lib/ -lcsdata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../lib -lcsdata
else:unix: LIBS += -L$$OUT_PWD/../../../lib -lcsdata

SOURCES += \
    $$PWD/csdataengineca.cpp

HEADERS += \
    $$PWD/csdataengineca.h

include(../../../epics.pri)
