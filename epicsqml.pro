TEMPLATE = lib
CONFIG += plugin
QT += quick

TARGET = pvcomponentsplugin
DESTDIR = PvComponents

include(src/plugin.pri)
include(PvComponents/qml.pri)
