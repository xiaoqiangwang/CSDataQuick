add_executable(test_qcsdata
    test_qcsdata.cpp
)
include_directories(
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

target_link_libraries(test_qcsdata
    csdata
)

SET_TARGET_PROPERTIES(test_qcsdata PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_executable(test_convert
    test_convert.cpp
)
include_directories(
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
    ${CMAKE_SOURCE_DIR}/src/libs/utils
)

target_link_libraries(test_convert
    csdata
    csutils
)

SET_TARGET_PROPERTIES(test_convert PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_executable(test_parser
    test_parser.cpp
)
include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/libs/ADLParser
    ${CMAKE_SOURCE_DIR}/src/libs/EDLParser
    ${CMAKE_SOURCE_DIR}/src/libs/UIParser
)

target_link_libraries(test_parser
    ADLParser
    EDLParser
    UIParser
    Qt5::Core
)

SET_TARGET_PROPERTIES(test_parser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
