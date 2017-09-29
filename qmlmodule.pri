# Adapted from mkspecs/features/qml_module.prf

isEmpty(TARGETPATH): error("Must set TARGETPATH (QML import name)")

for(qmlf, QML_FILES): fq_qml_files += $$absolute_path($$qmlf, $$_PRO_FILE_PWD_)

# These rules are to copy qml file to TARGETPATH
defineReplace(qmlModStripSrcDir) {
    return($$relative_path($$1, $$_PRO_FILE_PWD_))
}

qmlfiles2build.input = fq_qml_files
qmlfiles2build.output = $$top_builddir/qml/$$TARGETPATH/${QMAKE_FUNC_FILE_IN_qmlModStripSrcDir}
qmlfiles2build.commands = $$QMAKE_COPY ${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
qmlfiles2build.depends = $$QML_FILES
qmlfiles2build.CONFIG = no_link target_predeps

QMAKE_EXTRA_COMPILERS += qmlfiles2build

qmlfiles.base = $$_PRO_FILE_PWD_
qmlfiles.files = $$fq_qml_files
qmlfiles.path = $$PREFIX/qml/$$TARGETPATH
INSTALLS += qmlfiles
export(INSTALLS)
