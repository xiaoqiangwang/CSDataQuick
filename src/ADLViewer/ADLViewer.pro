TEMPLATE = app
QT += widgets quick

DESTDIR = ../../bin/

HEADERS += \
    viewer.h \
    ipcserver.h

SOURCES += \
    viewer.cpp \
    ipcserver.cpp \
    main.cpp

DEPENDPATH += . ../ADLParser
INCLUDEPATH += ../ADLParser
LIBS += -L ../ADLParser -lADLParser

mac {
    CONFIG -= app_bundle
    LIBS += -framework Security
}
