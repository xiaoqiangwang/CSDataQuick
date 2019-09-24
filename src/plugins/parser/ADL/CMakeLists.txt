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

target_compile_definitions(ADLParser PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_link_libraries(ADLParser PRIVATE
    Qt5::Core parser
)

set_target_properties(ADLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(ADLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS ADLParser DESTINATION plugins/parser)
