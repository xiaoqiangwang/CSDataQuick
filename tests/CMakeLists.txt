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

add_executable(test_edlparser
    test_edlparser.cpp
)
include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/libs/EDLParser
)

target_link_libraries(test_edlparser
    EDLParser
)

SET_TARGET_PROPERTIES(test_edlparser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
