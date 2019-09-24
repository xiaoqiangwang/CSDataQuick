set(SOURCES
    edl_parser.cpp
    EDLParser.cpp
)

set(HEADERS
    EDLParser.h
)

add_library(EDLParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(EDLParser PRIVATE
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

target_compile_definitions(EDLParser PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_link_libraries(EDLParser PRIVATE
    Qt5::Core
    parser
)

set_target_properties(EDLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(EDLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS EDLParser DESTINATION plugins/parser)
