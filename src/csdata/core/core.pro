TEMPLATE = lib
CONFIG += qt
TARGET = csdata
DESTDIR = $$OUT_PWD/../../../bin

QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/

SOURCES += \
    $$PWD/csdata.cpp \
    $$PWD/csdataengine.cpp \
    $$PWD/csdataenginesim.cpp \
    $$PWD/csdataenginemanager.cpp \
    $$PWD/objectmodel.cpp

HEADERS += \
    $$PWD/csdata.h \
    $$PWD/csdataengine.h \
    $$PWD/csdataenginesim.h \
    $$PWD/csdataenginemanager.h \
    $$PWD/objectmodel.h
