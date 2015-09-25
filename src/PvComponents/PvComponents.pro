TEMPLATE = lib
CONFIG += qt plugin
QT += quick printsupport
ios {
    DEFINES += NO_PRINTERSUPPORT
}
TARGET = pvcomponentsplugin
TARGETPATH = ../../imports/PvComponents/
DESTDIR = $${TARGETPATH}

include(qml.pri)
include(plugin.pri)
