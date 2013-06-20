
TEMPLATE = lib
CONFIG += qt plugin
QT += quick
DESTDIR = PvComponents

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

SOURCES += pvobject.cpp \
    pvobject_plugin.cpp

HEADERS += \
    pvobject.h \
    pvobject_plugin.h

OTHER_FILES += \
    PvComponents/CaBar.qml \
    PvComponents/CaChoiceButton.qml \
    PvComponents/CaMenu.qml \
    PvComponents/CaMessageButton.qml \
    PvComponents/CaOval.qml \
    PvComponents/CaSlider.qml \
    PvComponents/CaTextEntry.qml \
    PvComponents/CaTextLabel.qml \
    PvComponents/qmldir
