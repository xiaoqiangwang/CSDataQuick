set(SOURCES
    plugin.cpp
    sortfilterproxymodel.cpp
)

set(HEADERS
    plugin.h
    sortfilterproxymodel.h
)

add_library(csdataplugin MODULE
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ../csdata/core/
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
)

target_link_libraries(csdataplugin csdata)
target_link_libraries(csdataplugin Qt5::Core Qt5::Qml)
set_target_properties(csdataplugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Data/)

set(QML_JS_FILES
    qmldir
    plugins.qmltypes
)

foreach(FILE ${QML_JS_FILES})
    # make subdirectory if not existing
    if (${CMAKE_MAJOR_VERSION} LESS 3)
        get_filename_component(DIR ${FILE} PATH)
    else ()
        get_filename_component(DIR ${FILE} DIRECTORY)
    endif()

    if (NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Data/${DIR})
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Data/${DIR})
    endif()
    # copy qml files prebuild
    add_custom_command(
        TARGET csdataplugin PRE_BUILD    
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Data/${DIR}
        DEPENDS ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Data/${FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

install(TARGETS csdataplugin DESTINATION qml/CSDataQuick/Data)
install(FILES ${QML_JS_FILES} DESTINATION qml/CSDataQuick/Data)
