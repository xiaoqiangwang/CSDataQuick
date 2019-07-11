set(SOURCES
    csdataenginesim.cpp
)

set(HEADERS
    csdataenginesim.h
)

add_library(csdataenginesim MODULE STATIC
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

add_definitions(-DQT_STATICPLUGIN)

target_link_libraries(csdataenginesim Qt5::Core)
