set(SOURCES
    csdataengineca.cpp
)

set(HEADERS
    csdataengineca.h
)

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKEMACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif()

add_library(csdataengineca SHARED
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

