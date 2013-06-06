# Add more folders to ship with the application, here
folder_01.source = qml/qml2
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

INCLUDEPATH += $$(EPICS_BASE)/include/
LIBS += -L$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH) -lca -lCom
win32 {
    INCLUDEPATH += $$(EPICS_BASE)/include//os/WIN32
    LIBS += ws2_32
}
linux {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Linux
}
macx {
    INCLUDEPATH += $$(EPICS_BASE)/include/os/Darwin
}
# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

SOURCES += pvobject.cpp

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    pvobject.h

OTHER_FILES += \
    qml/qml2/CaTextEntry.qml
