TEMPLATE = lib
CONFIG += qt plugin
QT += quick
TARGET = csdataplugin
TARGETPATH = ../../imports/CSDataQuick/Data/
DESTDIR = $${TARGETPATH}

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

SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/csdata.cpp \
    $$PWD/csdataengine.cpp \
    $$PWD/csdataenginesim.cpp \
    $$PWD/csdataenginemanager.cpp \
    $$PWD/csdataengineca.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/csdata.h \
    $$PWD/csdataengine.h \
    $$PWD/csdataenginesim.h \
    $$PWD/csdataenginemanager.h \
    $$PWD/csdataengineca.h

OTHER_FILES = \
    $$PWD/qmldir \
    $$PWD/plugins.qmltypes

# These rules are to copy qml file to TARGETPATH
defineReplace(qmlModStripSrcDir) {
    return($$TARGETPATH/$$relative_path($$1, $$_PRO_FILE_PWD_))
}

qmlfiles2build.input = OTHER_FILES
qmlfiles2build.output_function = qmlModStripSrcDir
qmlfiles2build.commands = $$QMAKE_COPY ${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
qmlfiles2build.depends = $$QML_FILES
qmlfiles2build.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += qmlfiles2build
