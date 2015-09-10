file (GLOB QML_JS_FILES *.qml *.js qmldir)
macro(COPY_QML_FILES)
    foreach(file IN LISTS ARGN)
        execute_process({CMAKE_COMMAND} -E ${file} ${CMAKE_BINARY_DIR}/PvComponents)
    endforeach()
endmacro()

add_custom_target (PvComponents ALL 
    COMMAND
    SOURCES ${QML_JS_FILES}
)
set(SOURCES
    pvobject.cpp
    adimageprovider.cpp
    enums.cpp
    plugin.cpp
    utils.cpp
    shapes.cpp
    conversion.c
    qcustomplot.cpp
    plotitem.cpp
    baseitem.cpp
)

set(HEADERS
    pvobject.h
    adimageprovider.h
    enums.h
    plugin.h
    utils.h
    shapes.h
    qcustomplot.h
    plotitem.h
)

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKEMACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif()

add_library(pvcomponentsplugin SHARED
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5PrintSupport_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
)

target_link_libraries(pvcomponentsplugin Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::PrintSupport)
target_link_libraries(pvcomponentsplugin ${EPICS_LIBRARIES})
set_target_properties(pvcomponentsplugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/PvComponents)
