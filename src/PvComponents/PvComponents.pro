TEMPLATE = lib
CONFIG += qt plugin
QT += quick printsupport

TARGET = pvcomponentsplugin
TARGETPATH = ../../imports/PvComponents/
DESTDIR = $${TARGETPATH}

include(qml.pri)
include(plugin.pri)
