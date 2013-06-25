
TEMPLATE = lib
CONFIG += plugin
QT += quick

TARGET = pvcomponentsplugin
DESTDIR = ../PvComponents

# EPICS related
INCLUDEPATH += $$(EPICS_BASE)/include/
LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom
win32 {
    INCLUDEPATH += $$(EPICS_BASE)/include//os/WIN32
        LIBS += ws2_32
}
linux {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Linux
}
macx {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Darwin
}

SOURCES += \
    pvobject.cpp \
    adimageprovider.cpp \
    plugin.cpp

HEADERS += \
    pvobject.h \
    adimageprovider.h \
    plugin.h
