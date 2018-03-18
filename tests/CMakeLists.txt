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
