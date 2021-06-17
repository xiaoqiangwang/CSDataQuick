csdq_add_executable(Viewer
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
        csparser
)

target_include_directories(Viewer PRIVATE
    ${CMAKE_BINARY_DIR}/src/include
)

if(APPLE)
    find_library(OSXSecurity Security)
    target_link_libraries(Viewer PRIVATE ${OSXSecurity})
endif()
