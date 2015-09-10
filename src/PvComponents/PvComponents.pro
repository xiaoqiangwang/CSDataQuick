TEMPLATE = lib
CONFIG += qt plugin
QT += quick printsupport

TARGET = pvcomponentsplugin
TARGETPATH = PvComponents/Controls

include(qml.pri)
include(plugin.pri)
