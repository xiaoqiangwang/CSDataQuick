set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

add_library(ADLParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(ADLParser Qt5::Core)
target_link_libraries(ADLParser parser)

set_target_properties(ADLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(ADLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS ADLParser DESTINATION plugins/parser)
