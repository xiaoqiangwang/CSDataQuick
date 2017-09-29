set(SOURCES
    csdataengineca.cpp
)

set(HEADERS
    csdataengineca.h
)

add_library(csdataengineca MODULE
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ../core
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
)

add_definitions(${EPICS_DEFINITIONS})

target_link_libraries(csdataengineca Qt5::Core)
target_link_libraries(csdataengineca ${EPICS_LIBRARIES})
target_link_libraries(csdataengineca csdata)
set_target_properties(csdataengineca PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataengineca PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataengineca DESTINATION plugins/csdataengine)
