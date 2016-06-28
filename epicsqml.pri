!isEmpty(EPICSQML_PRI_INCLUDED):error("epicsqml.pri already included")
EPICSQML_PRI_INCLUDED = 1

defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

isEmpty(LIBRARY_BASENAME) {
    LIBRARY_BASENAME = lib
}

SOURCE_TREE = $$PWD
isEmpty(BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    BUILD_TREE = $$clean_path($$OUT_PWD)
    BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}

APP_PATH = $$BUILD_TREE/bin
osx {
    APP_TARGET = "ADL Viewer"
    APP_BUNDLE = $$APP_PATH/$${APP_TARGET}.app
    OUTPUT_PATH = $$APP_BUNDLE/Contents

    LIBRARY_PATH = $$OUTPUT_PATH/Frameworks
    PLUGIN_PATH  = $$OUTPUT_PATH/PlugIns

    LINK_LIBRARY_PATH = $$APP_BUNDLE/Contents/Frameworks
    LINK_PLUGIN_PATH  = $$APP_BUNDLE/Contents/PlugIns
} else {
    APP_TARGET = adlviewer
    OUTPUT_PATH = $$BUILD_TREE

    LIBRARY_PATH = $$OUTPUT_PATH/$$LIBRARY_BASENAME/$$APP_TARGET
    PLUGIN_PATH  = $$LIBRARY_PATH/plugins
    BIN_PATH     = $$OUTPUT_PATH/bin
    win32: \
        LIBEXEC_PATH = $$OUTPUT_PATH/bin
    else: \
        LIBEXEC_PATH = $$OUTPUT_PATH/libexec/$$APP_TARGET

    LINK_LIBRARY_PATH = $$BUILD_TREE/$$LIBRARY_BASENAME/$$APP_TARGET
    LINK_PLUGIN_PATH  = $$LINK_LIBRARY_PATH/plugins

    INSTALL_LIBRARY_PAH = $$INSTALL_PREFIX/$$LIBRARY_BASENAME/$$APP_TARGET
    INSTALL_PLUGIN_PATH = $$INSTALL_LIBRARY_PATH/plugins
    win32: \
        INSTALL_LIBEXEC_PATH = $$INSTALL_PREFIX/bin
    else: \
        INSTALL_LIBEXEC_PATH = $$INSTALL_PREFIX/libexec/$$APP_TARGET

    INSTALL_BIN_PATH = $$INSTALL_PREFIX/bin
}
