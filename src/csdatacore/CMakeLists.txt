set(SOURCES
    csdata.cpp
    csdataengine.cpp
    csdataengineca.cpp
    csdataenginemanager.cpp
    csdataenginesim.cpp
    plugin.cpp
)

set(HEADERS
    csdata.h
    csdataengine.h
    csdataengineca.h
    csdataenginemanager.h
    csdataenginesim.h
    plugin.h
)

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKEMACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif()

add_library(csdataplugin SHARED
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
)

target_link_libraries(csdataplugin Qt5::Core Qt5::Qml)
target_link_libraries(csdataplugin ${EPICS_LIBRARIES})
set_target_properties(csdataplugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/imports/CSDataQuick/Data/)

set(QML_JS_FILES
    qmldir
)

foreach(FILE ${QML_JS_FILES})
    # make subdirectory if not existing
    if (${CMAKE_MAJOR_VERSION} LESS 3)
        get_filename_component(DIR ${FILE} PATH)
    else ()
        get_filename_component(DIR ${FILE} DIRECTORY)
    endif()

    if (NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/imports/CSDataQuick/Data/${DIR})
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/imports/CSDataQuick/Data/${DIR})
    endif()
    # copy qml files prebuild
    add_custom_command(
        TARGET csdataplugin PRE_BUILD    
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILE} ${CMAKE_BINARY_DIR}/imports/CSDataQuick/Data/${DIR}
        DEPENDS ${CMAKE_BINARY_DIR}/imports/CSDataQuick/Data/${FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

