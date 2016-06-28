include(epicsqml.pri)

#version check qt
!minQtVersion(5, 4, 0) {
    message("Cannot build with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.4.0.")
}

TEMPLATE = subdirs
SUBDIRS = src docs tools examples

