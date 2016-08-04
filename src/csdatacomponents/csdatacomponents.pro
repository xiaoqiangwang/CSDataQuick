TEMPLATE = lib
CONFIG += qt plugin
QT += quick printsupport
ios {
    DEFINES += NO_PRINTERSUPPORT
}
win32-msvc* {
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

TARGET = csdatacomponentsplugin
TARGETPATH = $$OUT_PWD/../../qml/CSDataQuick/Components
DESTDIR = $${TARGETPATH}

include(qml.pri)
include(plugin.pri)
include(../../rpath.pri)
