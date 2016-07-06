TEMPLATE = lib
TARGET = ADLParser
DESTDIR = $$OUT_PWD/../../lib
win32:DLLDESTDIR = $$OUT_PWD/../../bin
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
