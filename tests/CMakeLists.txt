# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(test_qcsdata
    test_qcsdata.cpp
)
add_test(test_qcsdata
    test_qcsdata
)
target_link_libraries(test_qcsdata
    Qt::Test
    csdata
)

add_executable(test_utils
    test_utils.cpp
)
add_test(test_utils
    test_utils
)
target_link_libraries(test_utils
    Qt::Test
    csdata
    csutils
)

add_executable(test_parser
    test_parser.cpp
)
target_link_libraries(test_parser
    Qt::Core
    csparser
)
