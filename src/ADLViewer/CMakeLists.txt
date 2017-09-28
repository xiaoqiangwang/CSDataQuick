set(CMAKE_AUTORCC ON)

set(SOURCES
    ipcserver.cpp
    main.cpp
)

set(HEADERS
    ipcserver.h
)

if (CMAKE_VERSION VERSION_LESS 3.0)
    qt5_add_resources(RESOURCES
        main.qrc
    )
else ()
    set(RESOURCES
        main.qrc
    )
endif()

if(APPLE)
add_executable(ADLViewer MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)
else()
add_executable(ADLViewer
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)
endif()

include_directories(
    ${Qt5Qt_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5PrintSupport_INCLUDE_DIRS}
    ../ADLParser
    ../csdata/core
)
target_link_libraries(ADLViewer
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Quick
    Qt5::PrintSupport
    ADLParser
)
if(APPLE)
    find_library(OSXSecurity Security)
    target_link_libraries(ADLViewer ${OSXSecurity})
endif()

SET_TARGET_PROPERTIES(ADLViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_BINARY_DIR}/bin)
