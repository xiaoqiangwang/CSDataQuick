TEMPLATE = app

QT += qml quick

CONFIG -= app_bundle

DESTDIR = ../../bin/

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$OUT_PWD/../../imports

# Default rules for deployment.
include(deployment.pri)

