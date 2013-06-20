
TEMPLATE = lib
CONFIG += plugin
QT += quick

TARGET = pvobjectplugin
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

# Copy qml files post build
win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $${PWD}
    PWD_WIN ~= s,/,\\,g
    for(FILE, OTHER_FILES){
        QMAKE_POST_LINK += $$quote(cmd /c copy /y $${PWD_WIN}\\$${FILE} $${DESTDIR_WIN}$$escape_expand(\\n\\t))
    }
}
unix {
    for(FILE, OTHER_FILES){
        QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${DESTDIR}$$escape_expand(\\n\\t))
    }
}
