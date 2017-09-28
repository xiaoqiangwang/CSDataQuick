include(../../rpath.pri)

TEMPLATE = app
QT += quick widgets

INCLUDEPATH += $$PWD/../ADLParser $$PWD/../csdata/core
LIBS += -L$$OUT_PWD/../../lib/ -lADLParser

DESTDIR = ../../bin/

HEADERS += \
    ipcserver.h

SOURCES += \
    ipcserver.cpp \
    main.cpp

mac {
    LIBS += -framework Security
}

RESOURCES += \
    main.qrc

