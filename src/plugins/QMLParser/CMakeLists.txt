set(SOURCES
    QMLParser.cpp
)

set(HEADERS
    QMLParser.h
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

add_library(QMLParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(QMLParser Qt5::Core)
target_link_libraries(QMLParser parser)

set_target_properties(QMLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(QMLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS QMLParser DESTINATION plugins/parser)
