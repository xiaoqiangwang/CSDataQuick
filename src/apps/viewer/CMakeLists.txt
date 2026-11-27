set(APP csdq_viewer)

csdq_add_executable(${APP}
    SOURCES
        ipcserver.h
        ipcserver.cpp
        main.cpp
    RESOURCES
        main.qrc
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

target_include_directories(${APP} PRIVATE
    ${CMAKE_BINARY_DIR}/src/include
)

if(APPLE)
    find_library(OSXSecurity Security)
    target_link_libraries(${APP} PRIVATE ${OSXSecurity})
endif()
