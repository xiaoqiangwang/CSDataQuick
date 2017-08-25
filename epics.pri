# EPICS related
INCLUDEPATH += $$(EPICS_BASE)/include/

win32-msvc* {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/WIN32
    INCLUDEPATH += $$(EPICS_BASE)/include/compiler/msvc
    LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom
    exists($$(EPICS_BASE)/bin/$$(EPICS_HOST_ARCH)/*.dll) {
        DEFINES += EPICS_CALL_DLL
    } else {
        QMAKE_CFLAGS_RELEASE += /MT
        QMAKE_CXXFLAGS_RELEASE += /MT
        QMAKE_CFLAGS_DEBUG += /MTd
        QMAKE_CXXFLAGS_DEBUG += /MTd
        LIBS += -lws2_32 -luser32
        DEFINES += EPICS_DLL_NO
    }
}

linux {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Linux
    INCLUDEPATH += $$(EPICS_BASE)/include/compiler/gcc
    LIBS += $$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)/libca.a
    LIBS += $$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)/libCom.a
    system("nm -u $$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)/libCom.a | grep rl_"): LIBS += -lreadline
}

macx {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Darwin
    INCLUDEPATH += $$(EPICS_BASE)/include/compiler/clang
    LIBS += $$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)/libca.a
    LIBS += $$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)/libCom.a
}

