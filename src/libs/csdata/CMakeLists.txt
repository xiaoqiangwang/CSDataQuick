set(SOURCES
    csdata.cpp
    csdataengine.cpp
    csdataenginemanager.cpp
    objectmodel.cpp
)

set(HEADERS
    csdata_global.h
    csdata.h
    csdataengine.h
    csdataenginemanager.h
    objectmodel.h
)

add_library(csdata SHARED
    ${SOURCES}
    ${HEADERS}
)

add_definitions(-DCSDATA_LIBRARY)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
)

target_link_libraries(csdata Qt5::Core)

set_target_properties(csdata PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS csdata
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
