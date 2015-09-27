# EPICS related
INCLUDEPATH += $$(EPICS_BASE)/include/
LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom

win32 {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/WIN32
    LIBS += -lws2_32
    QMAKE_CXXFLAGS += -D_MINGW -DEPICS_DLL_NO
}
linux {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Linux
    LIBS += -Wl,-rpath=$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)
}
macx {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Darwin
}

DEPENDPATH += . ../ADLParser
INCLUDEPATH += ../ADLParser
LIBS += -L ../ADLParser -lADLParser

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../ADLParser/ADLParser.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../ADLParser/libADLParser.a

SOURCES += \
    $$PWD/pvobject.cpp \
    $$PWD/adimageprovider.cpp \
    $$PWD/plugin.cpp \
    $$PWD/utils.cpp \
    $$PWD/shapes.cpp \
    $$PWD/conversion.c \
    $$PWD/qcustomplot.cpp \
    $$PWD/plotitem.cpp \
    $$PWD/baseitem.cpp \
    $$PWD/enums.cpp \
    $$PWD/csdatasource.cpp \
    $$PWD/csdata.cpp \
    $$PWD/windowmanager.cpp \
    $$PWD/geometryspecification.cpp

HEADERS += \
    $$PWD/pvobject.h \
    $$PWD/adimageprovider.h \
    $$PWD/plugin.h \
    $$PWD/utils.h \
    $$PWD/shapes.h \
    $$PWD/qcustomplot.h \
    $$PWD/plotitem.h \
    $$PWD/baseitem.h \
    $$PWD/enums.h \
    $$PWD/csdatasource.h \
    $$PWD/csdata.h \
    $$PWD/windowmanager.h
