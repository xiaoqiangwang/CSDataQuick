set(SOURCES
    parser.cpp
    parsermanager.cpp
)

set(HEADERS
    parser_global.h
    parser.h
    parsermanager.h
)

add_library(parser SHARED
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(parser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(parser PRIVATE PARSER_LIBRARY)

target_link_libraries(parser PUBLIC Qt5::Core)

set_target_properties(parser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS parser
    RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
)
