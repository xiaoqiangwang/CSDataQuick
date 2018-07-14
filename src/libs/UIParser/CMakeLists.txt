set(SOURCES
    ui4.cpp
    parser.cpp
    UIParser.cpp
)

set(HEADERS
    ui4.h
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

target_link_libraries(UIParser Qt5::Core)

set_target_properties(UIParser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS UIParser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
