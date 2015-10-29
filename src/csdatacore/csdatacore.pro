TEMPLATE = lib
CONFIG += qt plugin
QT += quick
TARGET = csdataplugin
TARGETPATH = $$OUT_PWD/../../imports/CSDataQuick/Data/
DESTDIR = $${TARGETPATH}

QMAKE_RPATHDIR = @executable_path

INCLUDEPATH += $$PWD/../csdata/core
DEPENDPATH += $$PWD/../csdata/core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../bin/ -lcsdata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../bin -lcsdata
else:unix: LIBS += -L$$OUT_PWD/../../bin/ -lcsdata

SOURCES += \
    $$PWD/plugin.cpp \
    $$PWD/sortfilterproxymodel.cpp

HEADERS += \
    $$PWD/plugin.h \
    $$PWD/sortfilterproxymodel.h

OTHER_FILES = \
    $$PWD/qmldir \
    $$PWD/plugins.qmltypes

# These rules are to copy qml file to TARGETPATH
defineReplace(qmlModStripSrcDir) {
    return($$TARGETPATH/$$relative_path($$1, $$_PRO_FILE_PWD_))
}

qmlfiles2build.input = OTHER_FILES
qmlfiles2build.output_function = qmlModStripSrcDir
qmlfiles2build.commands = $$QMAKE_COPY ${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
qmlfiles2build.depends = $$QML_FILES
qmlfiles2build.CONFIG = no_link no_clean target_predeps

QMAKE_EXTRA_COMPILERS += qmlfiles2build
