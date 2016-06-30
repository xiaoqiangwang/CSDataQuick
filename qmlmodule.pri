# These rules are to copy qml file to TARGETPATH
defineReplace(qmlModStripSrcDir) {
    return($$relative_path($$TARGETPATH/$$relative_path($$1, $$_PRO_FILE_PWD_),$$OUT_PWD))
}

qmlfiles2build.input = QML_FILES
qmlfiles2build.output_function = qmlModStripSrcDir
qmlfiles2build.commands = $$QMAKE_COPY ${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
qmlfiles2build.depends = $$QML_FILES
qmlfiles2build.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += qmlfiles2build
