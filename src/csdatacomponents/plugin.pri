INCLUDEPATH += $$PWD/../ADLParser
INCLUDEPATH += $$PWD/../csdata/core/
DEPENDPATH += $$PWD/../ADLParser

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib -lADLParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib -lADLParser
else:unix: LIBS += -L$$OUT_PWD/../../lib/ -lADLParser

SOURCES += \
    $$PWD/pvobject.cpp \
    $$PWD/adimageprovider.cpp \
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
    $$PWD/csimageitem.cpp

HEADERS += \
    $$PWD/pvobject.h \
    $$PWD/adimageprovider.h \
    $$PWD/plugin.h \
    $$PWD/utils.h \
    $$PWD/shapes.h \
    $$PWD/qcustomplot.h \
    $$PWD/plotitem.h \
    $$PWD/baseitem.h \
    $$PWD/enums.h \
    $$PWD/windowmanager.h \
    $$PWD/attribute.h \
    $$PWD/csimageitem.h

include(../../epics.pri)
