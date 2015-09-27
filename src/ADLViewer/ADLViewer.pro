TEMPLATE = app
QT += quick widgets

DESTDIR = ../../bin/

HEADERS += \
    ipcserver.h

SOURCES += \
    ipcserver.cpp \
    main.cpp

DEPENDPATH += . ../ADLParser
INCLUDEPATH += ../ADLParser
LIBS += -L ../ADLParser -lADLParser

mac {
    CONFIG -= app_bundle
    LIBS += -framework Security
}

RESOURCES += \
    main.qrc
