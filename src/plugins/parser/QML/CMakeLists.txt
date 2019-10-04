set(SOURCES
    QMLParser.cpp
)

set(HEADERS
    QMLParser.h
)

add_library(QMLParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(QMLParser PRIVATE
    Qt5::Core parser
)

set_target_properties(QMLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(QMLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS QMLParser DESTINATION plugins/parser)
