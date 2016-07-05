set(SOURCES
    csdata.cpp
    csdataengine.cpp
    csdataenginemanager.cpp
    csdataenginesim.cpp
    objectmodel.cpp
)

set(HEADERS
    csdata.h
    csdataengine.h
    csdataenginemanager.h
    csdataenginesim.h
    objectmodel.h
    cs_global.h
)

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKEMACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif()

add_library(csdata SHARED
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
)

target_link_libraries(csdata Qt5::Core)
target_link_libraries(csdata ${EPICS_LIBRARIES})
set_target_properties(csdata PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

