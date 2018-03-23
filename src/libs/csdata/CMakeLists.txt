set(SOURCES
    csdata.cpp
    csdataengine.cpp
    csdataenginemanager.cpp
    objectmodel.cpp
)

set(HEADERS
    csdata.h
    csdataengine.h
    csdataenginemanager.h
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

install(TARGETS csdata
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
