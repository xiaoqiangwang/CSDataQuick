# EPICS related
INCLUDEPATH += $$(EPICS_BASE)/include/
LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom

win32 {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/WIN32
    #LIBS += -lws2_32
    #QMAKE_CXXFLAGS += -D_MINGW -DEPICS_DLL_NO
}
linux {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Linux
    LIBS += -Wl,-rpath=$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)
}
macx {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Darwin
}

INCLUDEPATH += $$PWD/../ADLParser
DEPENDPATH += $$PWD/../ADLParser

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib -lADLParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib -lADLParser
else:unix: LIBS += -L$$OUT_PWD/../../lib/ -lADLParser

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
    $$PWD/windowmanager.cpp \
    $$PWD/geometryspecification.cpp \
    $$PWD/attribute.cpp

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
    $$PWD/windowmanager.h \
    $$PWD/attribute.h
