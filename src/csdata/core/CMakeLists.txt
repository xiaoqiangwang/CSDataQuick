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

add_library(csdata SHARED
    ${SOURCES}
    ${HEADERS}
)

add_definitions(-DQCS_EXPORT)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
)

target_link_libraries(csdata Qt5::Core)
set_target_properties(csdata PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

