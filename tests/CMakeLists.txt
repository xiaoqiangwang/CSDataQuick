add_executable(test_qcsdata
    test_qcsdata.cpp
)
target_include_directories(test_qcsdata PRIVATE
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

target_link_libraries(test_qcsdata
    Qt5::Core
    csdata
)

add_executable(test_convert
    test_convert.cpp
)
target_include_directories(test_convert PRIVATE
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
    ${CMAKE_SOURCE_DIR}/src/libs/utils
)

target_link_libraries(test_convert
    Qt5::Core
    csdata
    csutils
)

add_executable(test_parser
    test_parser.cpp
)
target_include_directories(test_parser PRIVATE
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

target_link_libraries(test_parser
    Qt5::Core
    parser
)
