TEMPLATE = app

QT += qml quick

DESTDIR = ../../bin/

SOURCES += main.cpp

RESOURCES += main.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$OUT_PWD/../../qml

# Default rules for deployment.
include(deployment.pri)

