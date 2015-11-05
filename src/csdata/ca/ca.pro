TEMPLATE = lib
CONFIG += qt plugin
TARGET = csdataengineca
DESTDIR = $$OUT_PWD/../../../plugins

QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/

# EPICS related
INCLUDEPATH += $$(EPICS_BASE)/include/
LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom

win32 {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/WIN32
}
linux {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Linux
    LIBS += -Wl,-rpath=$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)
}
macx {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Darwin
}

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../bin/ -lcsdata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../bin -lcsdata
else:unix: LIBS += -L$$OUT_PWD/../../../bin/ -lcsdata

SOURCES += \
    $$PWD/csdataengineca.cpp

HEADERS += \
    $$PWD/csdataengineca.h
