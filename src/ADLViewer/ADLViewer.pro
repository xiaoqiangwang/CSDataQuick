TEMPLATE = app
QT += quick widgets

DESTDIR = ../../bin/

HEADERS += \
    ipcserver.h

SOURCES += \
    ipcserver.cpp \
    main.cpp

mac {
    CONFIG -= app_bundle
    LIBS += -framework Security
}

RESOURCES += \
    main.qrc
