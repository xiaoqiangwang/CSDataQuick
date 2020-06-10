set(SOURCES
    parser.cpp
    parsermanager.cpp
)

set(HEADERS
    parser_global.h
    parser.h
    parsermanager.h
)

add_library(csparser SHARED
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(csparser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(csparser PRIVATE PARSER_LIBRARY)

target_link_libraries(csparser PUBLIC Qt5::Core)

set_target_properties(csparser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS csparser
    RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
)
