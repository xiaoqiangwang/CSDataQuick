TEMPLATE = lib
CONFIG += qt plugin c++11
QT += quick printsupport concurrent
ios {
    DEFINES += NO_PRINTERSUPPORT
}
win32-msvc* {
    DEFINES += _CRT_SECURE_NO_WARNINGS
}
macx {
    QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override
}

TARGET = csdatacomponentsplugin
TARGETPATH = CSDataQuick/Components
DESTDIR = $$top_builddir/qml/$${TARGETPATH}

include(qml.pri)
include(plugin.pri)
include(../../rpath.pri)
