set(APP csdq_viewer)

csdq_add_executable(${APP}
    SOURCES
        ipcserver.h
        ipcserver.cpp
        main.cpp
    RESOURCES
        main.qrc
    DEPENDS
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Network
        csutils
        csdata
        csparser
)

target_include_directories(${APP} PRIVATE
    ${CMAKE_BINARY_DIR}/src/include
)

if(APPLE)
    find_library(OSXSecurity Security)
    target_link_libraries(${APP} PRIVATE ${OSXSecurity})
endif()
