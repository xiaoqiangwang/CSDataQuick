TEMPLATE = lib
TARGET = ADLParser
DEFINES += QCS_EXPORT

osx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/

# cs_global.h
INCLUDEPATH += ../csdata/core/

HEADERS += \
    common.h \
    element.h \
    parser.h

SOURCES += \
    element.cpp \
    parser.cpp

include(../../deployment.pri)
