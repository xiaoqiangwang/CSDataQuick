set(SOURCES
    csdataenginelocal.cpp
)

set(HEADERS
    csdataenginelocal.h
)

add_library(csdataenginelocal MODULE STATIC
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

add_definitions(-DQT_STATICPLUGIN)

target_link_libraries(csdataenginelocal Qt5::Core)
