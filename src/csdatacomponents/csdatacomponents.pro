TEMPLATE = lib
CONFIG += qt plugin
QT += quick printsupport
ios {
    DEFINES += NO_PRINTERSUPPORT
}
TARGET = csdatacomponentsplugin
TARGETPATH = $$OUT_PWD/../../imports/CSDataQuick/Components
DESTDIR = $${TARGETPATH}

include(qml.pri)
include(plugin.pri)
