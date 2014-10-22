set(SOURCES
    pvobject.cpp
    adimageprovider.cpp
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
    plugin.h
    utils.h
    shapes.h
    qcustomplot.h
    plotitem.h
)

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
