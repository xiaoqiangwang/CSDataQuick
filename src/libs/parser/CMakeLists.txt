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

add_definitions(-DPARSER_LIBRARY)

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${EPICS_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
)

target_link_libraries(parser Qt5::Core)

set_target_properties(parser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS parser
    RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
)
