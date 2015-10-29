TEMPLATE = lib
CONFIG += qt
TARGET = csdata
DESTDIR = $$OUT_PWD/../../bin

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

SOURCES += \
    $$PWD/csdata.cpp \
    $$PWD/csdataengine.cpp \
    $$PWD/csdataenginesim.cpp \
    $$PWD/csdataenginemanager.cpp \
    $$PWD/csdataengineca.cpp \
    $$PWD/objectmodel.cpp

HEADERS += \
    $$PWD/csdata.h \
    $$PWD/csdataengine.h \
    $$PWD/csdataenginesim.h \
    $$PWD/csdataenginemanager.h \
    $$PWD/csdataengineca.h \
    $$PWD/objectmodel.h
