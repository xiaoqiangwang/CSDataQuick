set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
)

add_definitions(-DQCS_EXPORT)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

add_library(ADLParser SHARED
    ${SOURCES}
    ${HEADERS}
)

install(TARGETS ADLParser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
