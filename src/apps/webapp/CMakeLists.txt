set(APP webapp)

csdq_add_executable(${APP}
    SOURCES
        main.cpp
    DEPENDS
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickControls2
        Qt${QT_VERSION_MAJOR}::Network
        csutils
        csdata
        csparser
)


# qt_add_executable(${APP}
#     main.cpp

# )

# target_link_libraries(${APP}
#     PRIVATE
#              Qt${QT_VERSION_MAJOR}::Core
#              Qt${QT_VERSION_MAJOR}::Gui
#              Qt${QT_VERSION_MAJOR}::Widgets
#              Qt${QT_VERSION_MAJOR}::Qml
#              Qt${QT_VERSION_MAJOR}::Quick
#              Qt${QT_VERSION_MAJOR}::QuickControls2
#              Qt${QT_VERSION_MAJOR}::Network
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
