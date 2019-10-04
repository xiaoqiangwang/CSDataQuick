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

target_link_libraries(csdataplugin PRIVATE
    Qt5::Core Qt5::Qml csdata)

set_target_properties(csdataplugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/CSDataQuick/Data/)

if(APPLE)
    set_target_properties(csdataplugin PROPERTIES
        INSTALL_RPATH "@loader_path/../../../lib")
endif()

set(QML_JS_FILES
    qmldir
    plugins.qmltypes
)

foreach(FILE ${QML_JS_FILES})
    # make subdirectory if not existing
    get_filename_component(DIR ${FILE} DIRECTORY)

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

install(TARGETS csdataplugin DESTINATION ${KDE_INSTALL_QMLDIR}/CSDataQuick/Data)
install(FILES ${QML_JS_FILES} DESTINATION ${KDE_INSTALL_QMLDIR}/CSDataQuick/Data)
