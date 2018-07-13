set(SOURCES
    parser.cpp
    UIParser.cpp
)

set(HEADERS
    UIParser.h
    UIParser_global.h
)

add_definitions(-DUIPARSER_LIBRARY)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
)

add_library(UIParser SHARED
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(UIParser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS UIParser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
