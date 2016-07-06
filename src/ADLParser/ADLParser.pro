TEMPLATE = lib
TARGET = ADLParser
DESTDIR = $$OUT_PWD/../../lib
DEFINES += QCS_EXPORT

osx:QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/

HEADERS += \
    common.h \
    element.h \
    parser.h

SOURCES += \
    element.cpp \
    parser.cpp
