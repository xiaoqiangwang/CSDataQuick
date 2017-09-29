include(../../rpath.pri)

TEMPLATE = app
QT += quick widgets

INCLUDEPATH += $$PWD/../ADLParser $$PWD/../csdata/core
LIBS += -L$$top_builddir/lib/ -lADLParser

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

include(../../deployment.pri)
