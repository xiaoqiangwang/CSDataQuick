GET_TARGET_PROPERTY(QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake LOCATION)
EXEC_PROGRAM( ${QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_DOCS" OUTPUT_VARIABLE QT_DOC_DIR )
EXEC_PROGRAM( ${QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_BINS" OUTPUT_VARIABLE QT_BIN_DIR )

find_program(QDOC_EXECUTABLE qdoc HINTS ${QT_BIN_DIR})
if(QDOC_EXECUTABLE STREQUAL "QDOC_EXECUTABLE-NOTFOUND")
    message(WARNING "qdoc not found, documentation cannot be built")
else()
    file (GLOB QDOC_FILES *.qdocconf src/*.qdoc style/*)
    if (${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} VERSION_GREATER 5.10)
        set (QDOC_INCLUDE_PATHS -I${CMAKE_CURRENT_SOURCE_DIR})
        foreach (_dir IN LISTS Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS
                               Qt${QT_VERSION_MAJOR}Qml_INCLUDE_DIRS Qt${QT_VERSION_MAJOR}Quick_INCLUDE_DIRS)
            set (QDOC_INCLUDE_PATHS ${QDOC_INCLUDE_PATHS} -I${_dir})
        endforeach()
        foreach (_dir libs/parser libs/csdata libs/utils imports/components)
            set (QDOC_INCLUDE_PATHS ${QDOC_INCLUDE_PATHS} -I${CMAKE_SOURCE_DIR}/src/${_dir})
        endforeach()
    endif()
    foreach (_type online offline)
        add_custom_target(html-${_type}
            COMMAND ${CMAKE_COMMAND}
                -E env CSDATAQUICK_VERSION=${CSDATAQUICK_VERSION}
                ${QDOC_EXECUTABLE}
                --indexdir ${QT_DOC_DIR}
                --outputdir ${CMAKE_CURRENT_BINARY_DIR}/html-${_type}
                ${QDOC_INCLUDE_PATHS}
                ${CMAKE_CURRENT_SOURCE_DIR}/csdataquick-${_type}.qdocconf
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generate ${_type} documents..."
            SOURCES ${QDOC_FILES})
    endforeach()
    find_program(QHG_EXECUTABLE qhelpgenerator HINTS ${QT_BIN_DIR})
    if(QHG_EXECUTABLE STREQUAL "QHG_EXECUTABLE-NOTFOUND")
        message(WARNING "qhelpgenerator not found, qch documentation cannot be built")
    else()
        add_custom_target(docs
            COMMAND ${CMAKE_COMMAND}
                -E env
                ${QHG_EXECUTABLE}
                html-offline/csdataquick.qhp
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generate qch documents..."
            DEPENDS html-offline)
    endif()
endif()
