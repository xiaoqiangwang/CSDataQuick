set(SOURCES
    csdataengineca.cpp
)

set(HEADERS
    csdataengineca.h
)

add_library(csdataengineca MODULE STATIC
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

add_definitions(${EPICS_DEFINITIONS})
add_definitions(-DQT_STATICPLUGIN)

target_link_libraries(csdataengineca Qt5::Core)
target_link_libraries(csdataengineca ${EPICS_LIBRARIES})
