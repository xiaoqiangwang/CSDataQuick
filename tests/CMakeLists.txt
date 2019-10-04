add_executable(test_qcsdata
    test_qcsdata.cpp
)
target_link_libraries(test_qcsdata
    Qt5::Core
    csdata
)

add_executable(test_convert
    test_convert.cpp
)
target_link_libraries(test_convert
    Qt5::Core
    csdata
    csutils
)

add_executable(test_parser
    test_parser.cpp
)
target_link_libraries(test_parser
    Qt5::Core
    parser
)
