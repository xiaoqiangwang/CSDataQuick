set(APP webapp)

csdq_add_executable(${APP}
    SOURCES
        main.cpp
    DEPENDS
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Network
        csutils
        csdata
        csparser
)


# qt_add_executable(${APP}
#     main.cpp

# )

# target_link_libraries(${APP}
#     PRIVATE
#              Qt6::Core
#              Qt6::Gui
#              Qt6::Widgets
#              Qt6::Qml
#              Qt6::Quick
#              Qt6::QuickControls2
#              Qt6::Network
# )

add_custom_command(
    TARGET ${APP} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/csdq_webapp.html
            ${CMAKE_CURRENT_BINARY_DIR}/csdq_webapp.html
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/csdq_webapp.html
)

target_include_directories(${APP} PRIVATE
    ${CMAKE_BINARY_DIR}/src/include
)
