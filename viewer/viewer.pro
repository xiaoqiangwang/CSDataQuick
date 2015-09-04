TEMPLATE = app
QT += widgets quick

HEADERS += element.h \
    common.h \
    viewer.h \
    fileinfo.h \
    ipcserver.h

SOURCES += element.cpp \
    common.cpp \
    main.cpp \
    viewer.cpp \
    #test.cpp \
    fileinfo.cpp \
    ipcserver.cpp

mac {
    LIBS += -framework Security
    #CONFIG -= app_bundle
}
