if(NOT TARGET Qt6::qdoc)
    message(WARNING "qdoc not found, documentation cannot be built")
    return()
endif()
GET_TARGET_PROPERTY(QDOC_EXECUTABLE Qt6::qdoc IMPORTED_LOCATION)

set(QT_DOC_DIR "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_DOCS}")

file (GLOB QDOC_FILES *.qdocconf src/*.qdoc style/*)
set (QDOC_INCLUDE_PATHS -I${CMAKE_CURRENT_SOURCE_DIR})
foreach (_dir IN LISTS Qt6Core_INCLUDE_DIRS Qt6Gui_INCLUDE_DIRS
                        Qt6Qml_INCLUDE_DIRS Qt6Quick_INCLUDE_DIRS)
    set (QDOC_INCLUDE_PATHS ${QDOC_INCLUDE_PATHS} -I${_dir})
endforeach()
foreach (_dir libs/parser libs/csdata libs/utils imports/components)
    set (QDOC_INCLUDE_PATHS ${QDOC_INCLUDE_PATHS} -I${CMAKE_SOURCE_DIR}/src/${_dir})
endforeach()

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

if(NOT TARGET Qt6::qhelpgenerator)
    message(WARNING "qhelpgenerator not found, qch documentation cannot be built")
    return()
endif()
GET_TARGET_PROPERTY(QHG_EXECUTABLE Qt6::qhelpgenerator IMPORTED_LOCATION)

add_custom_target(docs
    COMMAND ${CMAKE_COMMAND}
        -E env
        ${QHG_EXECUTABLE}
        html-offline/csdataquick.qhp
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generate qch documents..."
    DEPENDS html-offline)
