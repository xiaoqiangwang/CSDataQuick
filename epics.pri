# EPICS related
INCLUDEPATH += $$(EPICS_BASE)/include/
LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom

win32 {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/WIN32
    exists($$(EPICS_BASE)/bin/$$(EPICS_HOST_ARCH)/*.dll) {
        DEFINES += EPICS_CALL_DLL
    }
}
win32-msvc* {
    INCLUDEPATH += $$(EPICS_BASE)/include/compiler/msvc
}

linux {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Linux
    INCLUDEPATH += $$(EPICS_BASE)/include/compiler/gcc
    LIBS += -Wl,-rpath=$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)
}
macx {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Darwin
    INCLUDEPATH += $$(EPICS_BASE)/include/compiler/clang
}

