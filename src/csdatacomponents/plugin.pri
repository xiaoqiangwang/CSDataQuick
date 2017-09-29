INCLUDEPATH += $$PWD/../ADLParser
INCLUDEPATH += $$PWD/../csdata/core/
DEPENDPATH += $$PWD/../ADLParser

LIBS += -L$$top_builddir/lib/ -lADLParser -lcsdata

SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/utils.cpp \
    $$PWD/shapes.cpp \
    $$PWD/conversion.c \
    $$PWD/qcustomplot.cpp \
    $$PWD/plotitem.cpp \
    $$PWD/baseitem.cpp \
    $$PWD/enums.cpp \
    $$PWD/windowmanager.cpp \
    $$PWD/geometryspecification.cpp \
    $$PWD/attribute.cpp \
    $$PWD/csimageitem.cpp \
    $$PWD/adimage.cpp \
    $$PWD/mjpeg.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/utils.h \
    $$PWD/shapes.h \
    $$PWD/qcustomplot.h \
    $$PWD/plotitem.h \
    $$PWD/baseitem.h \
    $$PWD/enums.h \
    $$PWD/windowmanager.h \
    $$PWD/attribute.h \
    $$PWD/csimageitem.h \
    $$PWD/adimage.h \
    $$PWD/mjpeg.h

include(../../epics.pri)

target.path = $$PREFIX/qml/$$TARGETPATH
INSTALLS += target
